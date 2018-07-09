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

#include "gamestate_mainmenu.h"

#include "../game/game.h"

#define MENU_FADEIN_TIME    5.00f
#define MENU_FADEOUT_TIME   5.00f
#define MENU_TEXT_YSTART    215.0f
#define MENU_TEXT_YSPACE    25.0f

float val1 = 0.0f, val2 = 0.0f;
bool joyUpPressed = false, joyDownPressed = false;

bool UpPressed = false;
bool DownPressed = false;

GameState_MainMenu::GameState_MainMenu( Strata::Application* appContext ) :
    GameState(appContext)
{
    // Constructor
    mModulateTimer = 0.0f;
    mLastMainFocus = 0UL;
    mFocusOption = 0UL;
    mCurrentState = STATE_TYPE_FADEIN;
    mCurrentMenu = MENU_TYPE_NONE;
    mTargetMenu = MENU_TYPE_MAIN;
    mAnimationTimer = 0.0f;
    mFadeInTime = MENU_FADEIN_TIME;
    mFadeOutTime = MENU_FADEOUT_TIME;
}

VOID GameState_MainMenu::OnEnter( VOID )
{
    mCurrentState = STATE_TYPE_FADEIN;
    mCurrentMenu = MENU_TYPE_MAIN;
    mTargetMenu = MENU_TYPE_NONE;
    mFadeInTime = MENU_FADEIN_TIME;
    mFadeOutTime = MENU_FADEOUT_TIME;
    mAnimationTimer = 0.0f;

    // Initialize sprite renderer
    mSpriteRenderer.Initialize();

    // Initlaize our sprites
    mBlackSprite.Initialize( 0.0f, 0.0f, 640.0f, 360.0f, 10, Strata::ResourceManager::GetTextureHandle( "Fade" ));
    mLogoSprite.Initialize( 120.0f, 0.0f, 400.0f, 200.0f, 1, Strata::ResourceManager::GetTextureHandle("Logo" ));
    mBkgSprite.Initialize( 0.0f, 0.0f, 640.0f, 360.0f, 0, Strata::ResourceManager::GetTextureHandle("MenuBkg" ));
    mPanelSprite.Initialize( 80.0f, 65.0f, 480.0f, 260.0f, 2, Strata::ResourceManager::GetTextureHandle( "HSPanel" ));
    mCaptionSprite.Initialize( 120.0f, 30.0f, 400.0f, 33.0f, 1, Strata::ResourceManager::GetTextureHandle( "HSCaption" ));

    // Initialize Alpha Factor
    mBlackSprite.SetAlphaFactor(255);

}
VOID GameState_MainMenu::OnExit( VOID ) { }
VOID GameState_MainMenu::OnSuspend( VOID ) { }
VOID GameState_MainMenu::OnResume( VOID )
{
    OnEnter();
}

Strata::COLOR GameState_MainMenu::ModulateFocus( VOID )
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

VOID GameState_MainMenu::HandleInput( VOID )
{
    // If we are not in the input state, then ignore input
    if( mCurrentState != STATE_TYPE_INPUT ) return;

    // Handle button presses
    Game * gameApp = ((Game*)mpAppContext);

    if( mCurrentMenu == MENU_TYPE_MAIN )
    {
        if( gameApp->GetInputManager().IsKeyPressed( KEY_LSTICK_UP ) == TRUE || gameApp->GetInputManager().IsKeyPressed( KEY_DUP ) == TRUE)
        {
            if( mFocusOption == 0 ) 
                mFocusOption = 3;
            else mFocusOption--;
            mModulateTimer = 0.0f;

            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_select");
        }
        else if( gameApp->GetInputManager().IsKeyPressed( KEY_LSTICK_DOWN ) == TRUE || gameApp->GetInputManager().IsKeyPressed( KEY_DDOWN ) == TRUE )
        {
            if( mFocusOption == 3 ) 
                mFocusOption = 0;
            else mFocusOption++;
            mModulateTimer = 0.0f;
            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_select");
        }

        if( gameApp->GetInputManager().IsKeyPressed( KEY_A ) == TRUE )
        {
            if( mFocusOption == 0 )
            {
                mModulateTimer = 0.0f;
                mAnimationTimer = 0.0f;
                mLastMainFocus = mFocusOption;
                mFocusOption = 0xFFFFFFFF;
                mTargetMenu = MENU_TYPE_GAME;
                mCurrentState = STATE_TYPE_FADEOUT;
                ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
            }
            else if( mFocusOption == 1 )
            {
                mModulateTimer = 0.0f;
                mAnimationTimer = 0.0f;
                mLastMainFocus = mFocusOption;
                mFocusOption = 0xFFFFFFFF;
                mTargetMenu = MENU_TYPE_HIGHSCORE;
                mCurrentState = STATE_TYPE_FADEOUT;
                ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
            }            
            else if( mFocusOption == 2 )
            {
                mModulateTimer = 0.0f;
                mAnimationTimer = 0.0f;
                mLastMainFocus = mFocusOption;
                mFocusOption = 0xFFFFFFFF;
                mTargetMenu = MENU_TYPE_CONTROLS;
                mCurrentState = STATE_TYPE_FADEOUT;
                ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
            }     
            else if( mFocusOption == 3 )
            {
                // Quit
                //appletExit();
            }                           
        }
    }
    else if( mCurrentMenu == MENU_TYPE_HIGHSCORE )
    {
        if( gameApp->GetInputManager().IsKeyPressed( KEY_A ) == TRUE || gameApp->GetInputManager().IsKeyPressed( KEY_B ) == TRUE )
        {
            mFocusOption = 0xFFFFFFFF;
            mTargetMenu = MENU_TYPE_MAIN;
            mCurrentState = STATE_TYPE_FADEOUT;
            mAnimationTimer = 0.0f;
            ((Game*)mpAppContext)->GetAudioEngine().PlaySound("btn_press");
        }
    }
}
VOID GameState_MainMenu::Update(FLOAT elapsedTime )
{
    // Increment our animation timer
    mAnimationTimer += elapsedTime;

    // Adjust our fader depending on the current state
    if( mCurrentState == STATE_TYPE_FADEIN )
    {
        // Create at color and initilaize it to transparent white
        float normalizedAlpha = mAnimationTimer / mFadeInTime;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert the normalized alpha into a byte
        BYTE alphaValue = (BYTE)(255.0f - normalizedAlpha * 255.0f );

        // Set our sprite color
        mBlackSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0x00 ) {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_FADEIN_WAIT;
        }
    }
    else if( mCurrentState == STATE_TYPE_FADEOUT )
    {
        // Create at color and initilaize it to transparent white
        float normalizedAlpha = mAnimationTimer / mFadeOutTime;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert the normalized alpha into a byte
        BYTE alphaValue = (BYTE)(normalizedAlpha * 255.0f );

        // Set our sprite color
        mBlackSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0xFF ) {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_FADEOUT_WAIT;
        }
    }
    else if( mCurrentState == STATE_TYPE_FADEOUT_WAIT )
    {
        // Perform our transition 
        if( mTargetMenu == MENU_TYPE_MAIN )
        {
            mFadeInTime = MENU_FADEIN_TIME;
            mCurrentMenu = mTargetMenu;
            mCurrentState = STATE_TYPE_FADEIN;
        }
        else if( mTargetMenu == MENU_TYPE_HIGHSCORE )
        {
            mFadeInTime = MENU_FADEIN_TIME;
            mCurrentMenu = mTargetMenu;
            mCurrentState = STATE_TYPE_FADEIN;
        }
        else if( mTargetMenu == MENU_TYPE_CONTROLS )
        {
            mFadeInTime = MENU_FADEIN_TIME;
            mCurrentMenu = mTargetMenu;
            mCurrentState = STATE_TYPE_FADEIN;
        }
        else if( mTargetMenu == MENU_TYPE_GAME )
        {
            Game * gameApp = (Game*)mpAppContext;
            if( gameApp ) gameApp->GetStateManager().PushState( "Game" );
        }

		// Reset our timer, we're not doing any fading or anyhting
		mAnimationTimer = 0.0f;
    }
    else if( mCurrentState == STATE_TYPE_FADEIN_WAIT )
    {
        if( mCurrentMenu == MENU_TYPE_MAIN )
        {
            mModulateTimer = 0.0f;
            mFocusOption = mLastMainFocus;
            mCurrentState = STATE_TYPE_INPUT;

        }
        else if( mCurrentMenu == MENU_TYPE_HIGHSCORE )
        {
            mModulateTimer = 0.0f;
            mFocusOption = 0;
            mCurrentState = STATE_TYPE_INPUT;
        }
        else if( mCurrentMenu == MENU_TYPE_CONTROLS)
        {
            mModulateTimer = 0.0f;
            mFocusOption = 0;
            mCurrentState = STATE_TYPE_INPUT;
        }

 		// Reset our timer, we're not doing any fading or anyhting
		mAnimationTimer = 0.0f;       
    }
	else if( mCurrentState == STATE_TYPE_INPUT )
	{
		// Reset our timer, we're not doing any fading or anyhting
		mAnimationTimer = 0.0f;
	}    

    // Update Modulate timer
    mModulateTimer += elapsedTime;
}
VOID GameState_MainMenu::Draw( VOID )
{
    // Setup sprite renderer
    mSpriteRenderer.BeginDraw( );

    mBkgSprite.Draw(mSpriteRenderer);
    mBlackSprite.Draw( mSpriteRenderer );

    // Grab a reference to our font renderer
    Game * pGame = (Game*)mpAppContext;
    Strata::SpriteFont& mFont = pGame->GetFont();

    // Draw some text in center of the screen
    if( mCurrentMenu == MENU_TYPE_MAIN )
    {
        mLogoSprite.Draw( mSpriteRenderer );

        mFont.SetScale( 0.25f );
        if( mFocusOption == 0 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 0, SPRITEFONT_ALIGN_CENTER, 5, "Play" );
        if( mFocusOption == 1 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 1, SPRITEFONT_ALIGN_CENTER, 5, "Scores" );
        if( mFocusOption == 2 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 2, SPRITEFONT_ALIGN_CENTER, 5, "Controls" );
        if( mFocusOption == 3 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        mFont.DrawText( mSpriteRenderer, 320.0f, MENU_TEXT_YSTART + MENU_TEXT_YSPACE * 3, SPRITEFONT_ALIGN_CENTER, 5, "Quit" );                        
    }
    else if( mCurrentMenu == MENU_TYPE_HIGHSCORE )
    {
        mPanelSprite.Draw( mSpriteRenderer );
        mCaptionSprite.Draw( mSpriteRenderer );
        mFont.SetColor( 255, 255, 255, 255 );
        mFont.SetScale( 0.25f );

        Game * gameApp = ((Game*)mpAppContext);
        for( DWORD x = 0; x < gameApp->mScoreEntryCount; x++ )
        {
            mFont.DrawText( mSpriteRenderer, 190.0f, 100.0f + x * 22.0f, SPRITEFONT_ALIGN_RIGHT, 5, "%d.", x + 1 );
            mFont.DrawText( mSpriteRenderer, 222.0f, 100.0f + x * 22.0f, SPRITEFONT_ALIGN_LEFT, 5, "%s", gameApp->mScoreEntries[x].Name );
            mFont.DrawText( mSpriteRenderer, 410.0f, 100.0f + x * 22.0f, SPRITEFONT_ALIGN_RIGHT, 5, "%d", gameApp->mScoreEntries[x].Score );
            mFont.DrawText( mSpriteRenderer, 430.0f, 100.0f + x * 22.0f, SPRITEFONT_ALIGN_LEFT, 5, "W%d", gameApp->mScoreEntries[x].Wave );
        }

        // if( mFocusOption == 0 ) mFont.SetColor( ModulateFocus() ); else mFont.SetColor( 255, 255, 255, 255 );
        // mFont.DrawText( mSpriteRenderer, 320.0f, 340.0f, SPRITEFONT_ALIGN_CENTER, 5, "Back To Main Menu" );
    }

    // Finalize our rendering
    mSpriteRenderer.EndDraw();

    // Render our sprites
    mSpriteRenderer.Render();
}