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
#include "game.h"
#include "../states/gamestate_test.h"
#include "../states/gamestate_intro.h"
#include "../states/gamestate_mainmenu.h"
#include "../states/gamestate_game.h"
#include "../states/gamestate_gameover.h"

BOOL Game::Initialize( VOID )
{
    Strata::ResourceManager::AddTexture( "DevLogo", 512.0f, 96.0f, "assets/textures/splash.png", "" );
    Strata::ResourceManager::AddTexture( "Logo", 400.0f, 200.0f, "assets/textures/logo.png" );
    Strata::ResourceManager::AddTexture( "Fade", 64.0f, 64.0f, "assets/textures/black.png" );
    Strata::ResourceManager::AddTexture( "MenuBkg", 640.0f, 360.0f, "assets/textures/menubkg.png" );
    Strata::ResourceManager::AddTexture( "HSPanel", 922.0f, 489.0f, "assets/textures/hspanel.png" );
    Strata::ResourceManager::AddTexture( "HSCaption", 402.0f, 33.0f, "assets/textures/highscores.png" );
	Strata::ResourceManager::AddTexture( "Paused", 261.0f, 68.0f, "assets/textures/paused.png" );
    Strata::ResourceManager::AddTexture( "Smoke", 128.0f, 128.0f, "assets/textures/particle.png" );
    Strata::ResourceManager::AddTexture( "BarBkg", 160.0f, 16.0f, "assets/textures/barbkg.png" );
    Strata::ResourceManager::AddTexture( "BarBody", 156.0f, 12.0f, "assets/textures/barbody.png" );
    Strata::ResourceManager::AddTexture( "GameSprites", 1024.0f, 1024.0f, "assets/textures/spritesheet.png", "assets/textures/spritesheet.map" );

    // Initialize our font
    mFont.LoadFont( "FontTexture", "assets/fonts/segoe_ui.fnt", "assets/fonts/segoe_ui.png");

    // Initialize our sound engine
    mAudioEngine.Initialize(32);

    // Load our music and sound assets
    mAudioEngine.LoadMusic( "music", "assets/sounds/bgm.wav" );
    mAudioEngine.LoadSound( "btn_press", "assets/sounds/button_press.wav" );
    mAudioEngine.LoadSound( "btn_select", "assets/sounds/button_select.wav" );
    mAudioEngine.LoadSound( "phaser", "assets/sounds/phaser.wav" );
    mAudioEngine.LoadSound( "rocket", "assets/sounds/rocket.wav" );
    mAudioEngine.LoadSound( "shield", "assets/sounds/shield.wav" );
    mAudioEngine.LoadSound( "explosion_death", "assets/sounds/explosion1.wav" );
    mAudioEngine.LoadSound( "explosion_hit", "assets/sounds/explosion2.wav" );
    mAudioEngine.LoadSound( "death", "assets/sounds/death.wav" );

    // Add all of our game states to our director class
    #ifdef NXDEBUG
        mDirector.Add("Test", new GameState_Test(this));
    #endif
    mDirector.Add("Intro", new GameState_Intro(this));
    mDirector.Add("MainMenu", new GameState_MainMenu(this));
    mDirector.Add("Game", new GameState_Game(this));
    mDirector.Add("GameOver", new GameState_GameOver(this));
    
    // Move to our first state
    mDirector.PushState("MainMenu");

    // Initialize Inputmnager
    mInputManager.Initialize( CONTROLLER_P1_AUTO );

    ParseHighScoreData();

    // Return succesfully
    return TRUE;
}

Game::~Game( VOID )
{
    std::cout << "Game Instance Destroyed" << std::endl;
}

VOID Game::ParseHighScoreData( VOID )
{
    mScoreEntryCount = 0;
    for( DWORD x = 0; x < SCORE_ENTRY_MAX; x++ )
    {
        sprintf( mScoreEntries[x].Name, "User %d", (int)x );
        mScoreEntries[x].Score = x * 10000;
        mScoreEntries[x].Wave = SCORE_ENTRY_MAX - x;

        mScoreEntryCount++;
    }
}

DWORD Game::Update( FLOAT elapsedTime )
{
    // Let the director know we are starting a new frame
    mDirector.FrameBegin();

    // Clamp our elapsed time
    if( elapsedTime > 0.2f ) elapsedTime = 0.2f;
	if( elapsedTime < 0.0f ) elapsedTime = 0.0f;

    // Pool for input
    mInputManager.Poll();

    // If the + button is pressed, then exit
    if( mInputManager.IsKeyPressed( KEY_MINUS )) {
        mRunning = false;
        return 0UL;
    }

    // Pass control over to the director
    mDirector.HandleInput();
    mDirector.Update( elapsedTime );
    
    return 1UL;

}
DWORD Game::Render( VOID )
{
    // Draw our game state
    mDirector.Draw();

    // Draw our frame rate
    #ifdef NXDEBUG
        mSpriteRenderer.BeginDraw( Strata::SpriteRenderer::SORTTYPE_BACKTOFRONT );
        mFont.SetColor( 255, 255, 255, 192 );
        mFont.SetScale( 0.1875 );
        mFont.DrawText(mSpriteRenderer, 10, 10, SPRITEFONT_ALIGN_LEFT, 6, "Debug Mode" );
        mFont.DrawText(mSpriteRenderer, 320, 10, SPRITEFONT_ALIGN_CENTER, 6, "InvadersNX" );
        mFont.DrawText(mSpriteRenderer, 630, 10, SPRITEFONT_ALIGN_RIGHT, 6, "FPS:  %4.2f", Time.GetFrameRate() );
        mSpriteRenderer.EndDraw();
        mSpriteRenderer.Render();
    #endif

    // Let the director know that we have finisehd a complete frame
    mDirector.FrameEnd();

    return 1UL;
}