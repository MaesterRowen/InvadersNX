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

#include "gamestate_gameover.h"
#include "../game/game.h"

#define GAMEOVER_FADEIN_TIME        1.0f
#define GAMEOVER_FADEOUT_TIME       1.0f

GameState_GameOver::GameState_GameOver( Strata::Application* appContext ) :
    Strata::GameState( appContext )
{
    mAnimationTimer = 0.0f;
    mCurrentState = STATE_TYPE_FADEIN;
    mHighScore = FALSE;
    
}
VOID GameState_GameOver::OnEnter( VOID )
{
    Game * pGame = (Game*)mpAppContext;
    if( pGame ) 
    {
        //BOOL ret = pGame->UpdateHighScoreData()
    }

    // Set our intial state so that the transition can begin
    mCurrentState = STATE_TYPE_FADEIN;
    mAnimationTimer = 0.0f;

    // Initialize sprite renderer
    mSpriteRenderer.Initialize();

    // Initialize our sprits
    mBlackSprite.Initialize( 0.0f, 0.0f, 640.0f, 360.0f, 10, Strata::ResourceManager::GetTextureHandle("Fade"));
    mBlackSprite.SetAlphaFactor( 255 );
}
VOID GameState_GameOver::OnExit( VOID )
{
    // Release our sprite renderer
    mSpriteRenderer.Release();
}
VOID GameState_GameOver::HandleInput( VOID )
{
    // Handle input here, if necessary
}
VOID GameState_GameOver::Update( FLOAT elapsedTime )
{
    // Increment our animation timer
    mAnimationTimer += elapsedTime;

    if( mCurrentState == STATE_TYPE_FADEIN )
    {
        // Calculate normalized alpha value
        float normalizedAlpha = mAnimationTimer / GAMEOVER_FADEIN_TIME;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert the normalized alpha into a byte
        BYTE alphaValue = (BYTE)( 255.0f - normalizedAlpha * 255.0f );
        mBlackSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0x00 ) 
        {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_WAIT;
        }
    }
    else if( mCurrentState == STATE_TYPE_WAIT )
    {
        if( mAnimationTimer > 3.0f )
        {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_FADEOUT;
        }
    }
    else if( mCurrentState == STATE_TYPE_FADEOUT )
    {
        // Calculate normalized alpha value
        float normalizedAlpha = mAnimationTimer / GAMEOVER_FADEIN_TIME;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert the normalized alpha into a byte
        BYTE alphaValue = (BYTE)( normalizedAlpha * 255.0f );
        mBlackSprite.SetAlphaFactor( alphaValue );       
        if( alphaValue == 0xFF ) 
        {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_END;
            Game * pGame = (Game*)mpAppContext;
            if( pGame ) pGame->GetStateManager().PopState();
        }        
    }
}
VOID GameState_GameOver::Draw( VOID )
{
    // Grab our font
    Game * pGame = (Game*)mpAppContext;
    Strata::SpriteFont& mFont = pGame->GetFont();

    // Start sprite renderer
    mSpriteRenderer.BeginDraw();

    // Draw our fader
    mBlackSprite.Draw( mSpriteRenderer );

    // Draw our text
    mFont.SetScale( 0.75f );
    mFont.SetColor( 255, 255, 255, 255 );
    if( mHighScore ) {
        mFont.DrawText( mSpriteRenderer, 320.0f, 180.0f, SPRITEFONT_ALIGN_CENTER, 5, "New High Score!" );
    } else {
        mFont.DrawText( mSpriteRenderer, 320.0f, 180.0f, SPRITEFONT_ALIGN_CENTER, 5, "Game Over" );
    }

    // Finish up rendering
    mSpriteRenderer.EndDraw();
    mSpriteRenderer.Render();
}