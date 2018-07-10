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

#include <strata/strata.h>
#include "gamestate_intro.h"
#include "../game/game.h"

// Hardcoded durations for animation events
#define INTRO_FADEIN_TIME           1.5f
#define INTRO_FADEOUT_TIME          1.5f
#define INTRO_MINIMUM_SPLASH_TIME   1.0f


GameState_Intro::GameState_Intro( Strata::Application * appContext ) : 
    GameState(appContext)
{
    // Constructor
    mAnimationTimer = 0.0f;
    mCurrentState = STATE_TYPE_FADEIN;
    mIsLoaded = false;

}
GameState_Intro::~GameState_Intro( VOID )
{

}
VOID GameState_Intro::OnEnter( VOID )
{
    // Set our initial state so taht the transition process can begin
    mCurrentState = STATE_TYPE_FADEIN;
    mAnimationTimer = 0.0f;

    // Initialize renderer
    mSpriteRenderer.Initialize();

    // Initialize our sprite that will be used for our logo
    mLogoSprite.Initialize( 233.0f, 162.0f, 174.0f, 36.0f, 1, Strata::ResourceManager::GetTextureHandle( "DevLogo" ) );

    // Set alpha of logo
    mLogoSprite.SetAlphaFactor( 0 );

    // Set the audio
    //Game * gameApp = (Game*)mpAppContext;
    //if( gameApp ) gameApp->GetAudioEngine().PlayMusic("music");

    //std::cout << "GameState_Intro::OnEnter" << std::endl;
}
VOID GameState_Intro::OnExit( VOID )
{
    mSpriteRenderer.Release();
}
VOID GameState_Intro::OnSuspend( VOID )
{

}
VOID GameState_Intro::OnResume( VOID )
{

}
VOID GameState_Intro::HandleInput( VOID )
{
    if( mCurrentState == STATE_TYPE_END )
    {
        Game * gameApp = (Game*)mpAppContext;
        gameApp->GetStateManager().PushState("MainMenu");
    }
}
VOID GameState_Intro::Update( FLOAT elapsedTime )
{
    // Increment our animation timer
    mAnimationTimer += elapsedTime;

    if( mCurrentState == STATE_TYPE_FADEIN )
    {
        float normalizedAlpha = mAnimationTimer / INTRO_FADEIN_TIME;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert normalized alpha into a byte
        BYTE alphaValue = (BYTE)(normalizedAlpha * 255.0f );

        // Update our logo color
        mLogoSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0xFF ) {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_WAIT;
        }
    }
    else if( mCurrentState == STATE_TYPE_WAIT )
    {
        if( mAnimationTimer >= INTRO_MINIMUM_SPLASH_TIME )
        {
            // Check to see if we are loaded
            mIsLoaded = true;
            mCurrentState = STATE_TYPE_FADEOUT;
            mAnimationTimer = 0.0f;
        }
    }
    else if( mCurrentState == STATE_TYPE_FADEOUT )
    {
        float normalizedAlpha = mAnimationTimer / INTRO_FADEIN_TIME;
        if( normalizedAlpha > 1.0f ) normalizedAlpha = 1.0f;

        // Convert normalized alpha into a byte
        BYTE alphaValue = (BYTE)(255 - normalizedAlpha * 255.0f );

        // Update our logo color
        mLogoSprite.SetAlphaFactor( alphaValue );
        if( alphaValue == 0x00 ) {
            mAnimationTimer = 0.0f;
            mCurrentState = STATE_TYPE_END;
        }      
    }

}
VOID GameState_Intro::Draw( VOID )
{
    mSpriteRenderer.BeginDraw();
    mLogoSprite.Draw( mSpriteRenderer );
    mSpriteRenderer.EndDraw();
    mSpriteRenderer.Render();
}