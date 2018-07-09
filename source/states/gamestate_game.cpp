// Invaders NX
// Copyright (C) 2018 MaesterRowen
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <iostream>
#include "../game/game.h"
#include "gamestate_game.h"

#define MENU_TEXT_YSTART    215.0f
#define MENU_TEXT_YSPACE    25.0f

GameState_Game::GameState_Game( Strata::Application* appContext ) :
    GameState(appContext)
{
    mBkgTexture = 0UL;
    mAtlasTexture = 0UL;
    mGUITexture = 0UL;
    mModulateTimer = 0.0f;
    mCurrentScore = 0UL;
    mFocusOption = 0;
    mCurrentWave = 0;
    mWaveAlphaValue = 0x00;
}

GameState_Game::~GameState_Game( VOID )
{
    // Destructor
}

VOID GameState_Game::OnExit( VOID )
{
    // Clean up
    mSpriteRenderer.Release();

    mProjectiles.clear();
}
VOID GameState_Game::OnEnter( VOID )
{
    mModulateTimer = 0.0f;
    mCurrentScore = 0UL;
    mFocusOption = 0;
    mCurrentWave = 1;
    mAnimationTimer = 0.0f;
    mCurrentState = STATE_TYPE_GAME_FADEIN;
    mWaveAlphaValue = 0x00;

    // Initailize Sprite Renderer
    mSpriteRenderer.Initialize();
    mInputManager.Initialize(CONTROLLER_P1_AUTO);

    
    // Preload some game artwork assets
    mBkgTexture = Strata::ResourceManager::GetTextureHandle( "MenuBkg" );
    mAtlasTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites" );
    mRocketIconTexture = Strata::ResourceManager::GetTextureHandle( mAtlasTexture, "RocketIcon" );


    // Intialize game objects
    mBlackSprite.Initialize( 0.0f, 0.0f, 640.0f, 360.0f, 10, Strata::ResourceManager::GetTextureHandle( "Fade" ));
    mPausedSprite.Initialize( 255.0f, 151.0f, 130.0f, 34.0f, 12, Strata::ResourceManager::GetTextureHandle( "Paused" ));

    mBlackSprite.SetAlphaFactor( 255 );

    // Background initalize
    mBkgSprite.Initialize( 0.0f, 0.0f, 640.0f, 360.0f, 0, mBkgTexture );

    // Player Initialization
    mPlayer.SetCanMove( true );
    mPlayer.Initialize( 320.0f, 320.0f, 2, mAtlasTexture );
    mPlayer.SetInputManager( &mInputManager );
    mPlayer.SetAudioEngine( &(((Game*)mpAppContext)->GetAudioEngine()) );
    mPlayer.AddRocket( 5 );
    mPlayer.AddHealthPoints( 100 );

    // Enemy Swam Initialization
    mSwarm.Initialize( 320.0f, 0.0f, mCurrentWave, mAtlasTexture );
    mSwarm.HookScoreCallback( GameState_Game::UpdateScoreHook, this );

    // UI Initalization
    mHealthBar.Initialize(      16.0f,               344.0f,               80.0f, 8.0f, 8, Strata::ResourceManager::GetTextureHandle( "BarBkg" ));
    mHealthBarBody.Initialize(  16.0f,               344.0f,               80.0f, 8.0f, 9, Strata::ResourceManager::GetTextureHandle( "BarBody" ));
    mShieldBar.Initialize(      104.0f,              344.0f,               80.0f, 8.0f, 8, Strata::ResourceManager::GetTextureHandle( "BarBkg" ));
    mShieldBarBody.Initialize(  104.0f,              344.0f,               80.0f, 8.0f, 9, Strata::ResourceManager::GetTextureHandle( "BarBody" ));

    // Set color factors
    mHealthBarBody.SetColorFactor( 30, 255, 0 );
    mShieldBarBody.SetColorFactor( 0, 152, 255 );

    mpSmokeParticles = new Strata::ParticleBatch();
    mpSmokeParticles->Initialize( 500, 1.0f, 3, Strata::ResourceManager::GetTextureHandle("Smoke"), [](Strata::Particle& p, FLOAT t) {
        p.Position.x += p.Velocity.x * t;
        p.Position.y += p.Velocity.y * t;

        p.Color.Components.a = (BYTE)(p.Life * 255.0f );
        p.Color.Components.g = 255 - (BYTE)(p.Life * 105.0f );
        p.Color.Components.b = 255 - (BYTE)(p.Life * 255.0f );
    });

    mParticleEngine.AddParticles(mpSmokeParticles);

    std::cout << "GameState_Game::OnEnter" << std::endl;
}

BOOL GameState_Game::CheckForDeath( VOID )
{
    if( mCurrentState != STATE_TYPE_WAVE_INPUT ) return FALSE;

    if( mPlayer.GetHealthPoints() > 0 ) { 
        return FALSE;
    } else {
        Game * game = (Game *)mpAppContext;
        game->GetAudioEngine().PlaySound("death");
        return TRUE;
    }
}

VOID GameState_Game::HandleInput( VOID )
{
    mInputManager.Poll();
    if( mCurrentState == STATE_TYPE_WAVE_INPUT )
    {
        if( mInputManager.IsKeyPressed( KEY_PLUS ) == TRUE )
        {
            Game * gameApp = (Game*)mpAppContext;
            if( gameApp) gameApp->GetAudioEngine().PlaySound("btn_press");
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_PAUSE_START;
        }
    }
    else if( mCurrentState == STATE_TYPE_PAUSE_INPUT )
    {
        if( mInputManager.IsKeyPressed( KEY_B ) == TRUE )
        {
            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_PAUSE_RETURN;
        }
        else if( mInputManager.IsKeyPressed( KEY_DDOWN) == TRUE || mInputManager.IsKeyPressed( KEY_LSTICK_DOWN ) == TRUE )
        {
            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_select");

            if( mFocusOption == 1 ) mFocusOption = 0;
            else mFocusOption++;
        }
        else if( mInputManager.IsKeyPressed( KEY_DUP) == TRUE || mInputManager.IsKeyPressed( KEY_LSTICK_UP ) == TRUE )
        {
            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_select");

            if( mFocusOption == 0) mFocusOption = 1;
            else mFocusOption--;
        }
        else if( mInputManager.IsKeyPressed( KEY_A ) == TRUE )
        {
            if( mFocusOption == 0 )
            {
                mAnimationTimer = 0.0f;
                ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
                mCurrentState = STATE_TYPE_PAUSE_RETURN;
            }
            else if( mFocusOption == 1 )
            {
                Game * gameApp = (Game*)mpAppContext;
                if( gameApp ) 
                {
                    gameApp->GetAudioEngine().PlaySound("btn_press");
                    gameApp->GetStateManager().PopState();
                }
            }
        }
    }
}
Strata::COLOR GameState_Game::ModulateFocus( VOID )
{
    Strata::COLOR focusColor;
    float multiplier = 0.33f * cos( 7 * mModulateTimer ) + 0.66f;
    if( multiplier > 1.0f ) multiplier = 1.0f;
    if( multiplier < 0.0f ) multiplier = 0.0f;

    focusColor.Components.a = 255;
    focusColor.Components.r = (BYTE)(255.0f * multiplier);
    focusColor.Components.g = (BYTE)(150.0f * multiplier);
    focusColor.Components.b = (BYTE)(0.0f * multiplier);

    return focusColor;
}
VOID GameState_Game::Update( FLOAT elapsedTime )
{
    bool paused = false;
    mAnimationTimer += elapsedTime;

    #ifdef NXDEBUG
        static DWORD lastState = 0xFFFFFFFF;
        if( mCurrentState != lastState ) {
            std::cout << "Changing State:  " << lastState << " to " << mCurrentState << std::endl;
            lastState = mCurrentState;
        }
    #endif

    if( mCurrentState == STATE_TYPE_GAME_FADEIN )
    {
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );

        float normalizedAlpha = mAnimationTimer / 2.0f;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        BYTE alphaValue = (BYTE)(255.0f - normalizedAlpha * 255.0f );
        mBlackSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0x00 ) {
            mAnimationTimer = 0.0f;
            mCurrentWave = 1;
            mWaveAlphaValue = 0x00;
            mCurrentState = STATE_TYPE_WAVE_INTRO;
        }
    } 
    else if( mCurrentState == STATE_TYPE_WAVE_INTRO )
    {
        mPlayer.ReleaseShield();
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );

        float normalizedAlpha = mAnimationTimer / 2.0f;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        mWaveAlphaValue = (BYTE)(normalizedAlpha * 255.0f );
        if( mWaveAlphaValue > 0x44 ) {
            mSwarm.UpdateIntro( elapsedTime );
        }

        if( mWaveAlphaValue == 0xFF ) {
            mAnimationTimer = 0.0f;
            mWaveAlphaValue = 0xFF;
            mCurrentState = STATE_TYPE_WAVE_OUTRO;
        }
    }
    else if( mCurrentState == STATE_TYPE_WAVE_OUTRO )
    {
        mPlayer.ReleaseShield();
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );

        // Continue updating our swarm
        mSwarm.UpdateIntro(elapsedTime);
        if( mSwarm.IsStartReady() == TRUE )
        {
            static float outroTimer = 0.0f;
            outroTimer += elapsedTime;

            float normalizedAlpha = outroTimer / 0.25f;
            if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

            mWaveAlphaValue = (BYTE)(255.0f - normalizedAlpha * 255.0f );

            if( mWaveAlphaValue == 0x00 ) {
                outroTimer = 0.0f;
                mAnimationTimer = 0.0f;
                mWaveAlphaValue = 0x00;
                mCurrentState = STATE_TYPE_WAVE_INPUT;
            }
        }       
    }
    else if( mCurrentState == STATE_TYPE_WAVE_INPUT )
    {
        mPlayer.SetCanShoot( true );
        mPlayer.SetCanShield( true );

        // check for collisisons
        mSwarm.CheckCollision( &(((Game*)mpAppContext)->GetAudioEngine()), mProjectiles, mExplosions );
        mPlayer.CheckCollision( mProjectiles, mExplosions, mSwarm );
        
        if( CheckForDeath() == TRUE )
        {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_GAME_FADEOUT;
            mPlayer.SetCanMove( false );
            mPlayer.SetCanShield( false );
            mPlayer.SetCanShoot( false );
        }

        // Check for wave cleared
        if( mSwarm.IsCleared() == TRUE )
        {
            mAnimationTimer = 0.0f;
            mWaveAlphaValue = 0x00;
            mCurrentWave++;             // Move to the next wave
            mCurrentState = STATE_TYPE_WAVE_INTRO;
        }
    }
    else if( mCurrentState == STATE_TYPE_PAUSE_START )
    {
        paused = true;
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );
        mPlayer.SetCanMove( false );

        float normalizedAlpha = mAnimationTimer / 0.25f;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        BYTE alphaValue = (BYTE)(normalizedAlpha * 200.0f );

        // Set our sprite color
        mBlackSprite.SetAlphaFactor( alphaValue );

        if( alphaValue == 200 ) {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_PAUSE_INPUT;
        }
    }
    else if( mCurrentState == STATE_TYPE_PAUSE_INPUT )
    {
        paused = true;
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );
        mPlayer.SetCanMove( false );
    }
    else if( mCurrentState == STATE_TYPE_PAUSE_RETURN )
    {
        paused = true;
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );
        mPlayer.SetCanMove( false );

        float normalizedAlpha = mAnimationTimer / 0.25f;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        BYTE alphaValue = (BYTE)(200.0f - normalizedAlpha * 200.0f );

        // Set our sprite color
        mBlackSprite.SetAlphaFactor( alphaValue );

        if( alphaValue == 0x00 ) {
            mAnimationTimer = 0.0f;
            paused = false;
            mPlayer.SetCanShoot( true );
            mPlayer.SetCanShield( true );
            mPlayer.SetCanMove( true );            
            mCurrentState = STATE_TYPE_WAVE_INPUT;
        }
    }    
    else if( mCurrentState == STATE_TYPE_GAME_FADEOUT )
    {
        mPlayer.SetCanMove( false );
        mPlayer.SetCanShoot( false );
        mPlayer.SetCanShield( false );

        if( mAnimationTimer >= 1.0f )
        {
            float fadeTimer = mAnimationTimer - 1.0f;

            // CAlculate normalized alpha into byte
            float normalizedAlpha = fadeTimer / 2.0f;
            if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

            // Convert to byte
            BYTE alphaValue = (BYTE)(normalizedAlpha * 255.0f );

            // Set our sprite alpha
            mBlackSprite.SetAlphaFactor( alphaValue );
            if( alphaValue == 0xFF )
            {
                mAnimationTimer = 0.0f;
                mWaveAlphaValue = 0x00;

                // now we pop some states
                Game * gameApp = (Game*)mpAppContext;
                if( gameApp ) 
                {
                    gameApp->GetStateManager().PopState();
                    gameApp->GetStateManager().PushState( "GameOver" );
                }
            }
        }
    }
    
    if( paused == false )
    {
        // Update hte player
        mPlayer.Update(mpSmokeParticles, mProjectiles, elapsedTime);

        // Update the swarm
        if( mCurrentState == STATE_TYPE_WAVE_INPUT || mCurrentState == STATE_TYPE_GAME_FADEOUT ) {
            mSwarm.Update( mProjectiles, elapsedTime );
        }

        // Update our other things
        updateBullets( elapsedTime );
        updateExplosions( elapsedTime );
        updatePowerups( elapsedTime );

        // Udpate particle engine
        mParticleEngine.Update(elapsedTime);
    }

	// Fix up health bar
	FLOAT healthPercent = (FLOAT)(mPlayer.GetHealthPoints()) / 100.0f;
	mHealthBarBody.SetWidth( 80.0f * healthPercent );

	FLOAT shieldPercent = (FLOAT)(mPlayer.GetShieldRemaining()) / 100.0f;
	mShieldBarBody.SetWidth( 80.0f * shieldPercent );

	// Update rocket list
	mUIRockets.clear();
	for( DWORD x = 0; x < mPlayer.GetRocketsRemaining(); x++ )
	{
		Strata::Sprite rocket;
		rocket.Initialize( (624.0f - mPlayer.GetRocketsRemaining() * 14.0f) + (14.0f * x), 340.0f, 12.0f, 18.0f, 3, mRocketIconTexture );
		mUIRockets.emplace_back( rocket );
	}

	mModulateTimer += elapsedTime;    

}
VOID GameState_Game::Draw( VOID )
{
    // Draw the background
    mSpriteRenderer.BeginDraw( );
    mBkgSprite.Draw( mSpriteRenderer );
    mSpriteRenderer.EndDraw();
    mSpriteRenderer.Render();

    // Grab a reference to our font renderer
    Game * pGame = (Game*)mpAppContext;
    Strata::SpriteFont& mFont = pGame->GetFont();

    // Draw our game objects
    mSpriteRenderer.BeginDraw( );

    // Draw our black sprite
    mBlackSprite.Draw( mSpriteRenderer );

    if( mCurrentState == STATE_TYPE_WAVE_INTRO || mCurrentState == STATE_TYPE_WAVE_OUTRO )
    {
        mFont.SetScale( 0.5f );
        mFont.SetColor( 255, 255, 255, mWaveAlphaValue );
        mFont.DrawText( mSpriteRenderer, 320, 180, SPRITEFONT_ALIGN_CENTER, 5, "Wave %d", mCurrentWave );
    }

    if( mCurrentState == STATE_TYPE_PAUSE_INPUT )
    {
        mPausedSprite.Draw(mSpriteRenderer);

        // Draw our menu items
        mFont.SetScale( 0.25f );
        if( mFocusOption == 0 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 0, SPRITEFONT_ALIGN_CENTER, 12, "Continue" );
        if( mFocusOption == 1 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 1, SPRITEFONT_ALIGN_CENTER, 12, "Quit" );
    }

    // Draw the player
    if( mCurrentState != STATE_TYPE_GAME_FADEOUT ) {
        mPlayer.Draw( mSpriteRenderer );
    }

    // Draw the health and shield bar
    mHealthBarBody.Draw( mSpriteRenderer );
    mHealthBar.Draw( mSpriteRenderer );
    mShieldBarBody.Draw( mSpriteRenderer );
    mShieldBar.Draw( mSpriteRenderer );

    // Draw the rocket UI
    for( DWORD x = 0; x < mUIRockets.size(); x++ ) {
        mUIRockets[x].Draw(mSpriteRenderer );
    }

    // Draw the Enemy Swarm
    mSwarm.Draw( mSpriteRenderer, &mFont );

    // Draw explosions
    for( DWORD x = 0; x < mExplosions.size(); x++ ) {
        mExplosions[x].Draw(mSpriteRenderer);
    }

    // TODO Draw projectiles
    for( DWORD x = 0; x < mProjectiles.size(); x++ ) {
        mProjectiles[x].Draw(mSpriteRenderer);
    }

    // Draw Score
    mFont.SetScale( 0.2f );
    mFont.SetColor( 255, 255, 255, 255 );
    mFont.DrawText( mSpriteRenderer, 320.0f, 20.0f, SPRITEFONT_ALIGN_CENTER, 4, "Score: %d", mCurrentScore );

    #ifdef NXDEBUG
        // Draw soem debug text
        mFont.SetScale( 0.1875f );
        mFont.SetColor( 255, 255, 0, 192 );
        mFont.DrawText( mSpriteRenderer, 630.0f, 30.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Particles:  %d", mParticleEngine.GetParticleCount() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 45.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Target Y:  %4.2f", mSwarm.GetTargetY() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 60.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Transition Y:  %4.2f", mSwarm.GetTransitionY() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 75.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Swarm Pos (%4.2f, %4.2f)", mSwarm.GetX(), mSwarm.GetY() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 90.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Swarm Bounds (L: %4.2f, R: %4.2f, T: %4.2f, B: %4.2f", mSwarm.GetBounds().left, mSwarm.GetBounds().right, mSwarm.GetBounds().top, mSwarm.GetBounds().bottom);
        mFont.DrawText( mSpriteRenderer, 630.0f, 105.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Projectiles:  %d", mProjectiles.size() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 120.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Can Move:  %d", mPlayer.CanMove() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 135.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Can Shoot:  %d", mPlayer.CanShoot() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 150.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Can Shield:  %d", mPlayer.CanShield() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 165.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Player Position: (%4.2f, %4.2f)", mPlayer.GetX(), mPlayer.GetY() );
        mFont.DrawText( mSpriteRenderer, 630.0f, 180.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Wave Count:  %d", mCurrentWave );
        mFont.DrawText( mSpriteRenderer, 630.0f, 195.0f, SPRITEFONT_ALIGN_RIGHT, 20, "Bottom Start:  %4.2f", mSwarm.GetBottomStart() );
    #endif


    // Draw center lines of player
    #ifdef NXDEBUG
        Strata::COLOR col; col.AsDWORD = MAKE_RGBA( 128, 0, 0, 255 );
        mSpriteRenderer.DrawLine( 0.0f, mPlayer.GetY(), 640.0f, mPlayer.GetY(), col);
        mSpriteRenderer.DrawLine( mPlayer.GetX(), 0.0f, mPlayer.GetX(), 360.0f, col );
    #endif

    mSpriteRenderer.EndDraw();
    mSpriteRenderer.Render();

    // Render particles
    mParticleEngine.Draw( mSpriteRenderer );
}

VOID GameState_Game::updateBullets( FLOAT elapsedTime )
{
    for( DWORD i = 0; i < mProjectiles.size(); )
    {
        if( mProjectiles[i].Update( 0.0f, 640.0f, -32.0f, 392.0f, elapsedTime ) )
        {
            // Bullet ocllided with edge of screen
            mProjectiles[i] = mProjectiles.back();
            mProjectiles.pop_back();
        }
        else 
        {
            i++;
        }
    }
}
VOID GameState_Game::updateExplosions( FLOAT elapsedTime )
{
    for( DWORD i = 0; i < mExplosions.size(); )
    {
        if( mExplosions[i].Update( elapsedTime ))
        {
            mExplosions[i] = mExplosions.back();
            mExplosions.pop_back();
        }
        else 
        {
            i++;
        }
    }

}
VOID GameState_Game::updatePowerups( FLOAT elapsedTime )
{

}