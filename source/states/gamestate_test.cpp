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
#include "gamestate_test.h"

    
GameState_Test::GameState_Test( Strata::Application * appContext ) : 
    GameState(appContext)
{

    // Create our player
    //mPlayer.Initialize();
}
GameState_Test::~GameState_Test( VOID )
{
    std::cout << "GameState_Test - Destroyed " << std::endl;

}

VOID GameState_Test::OnEnter( VOID )
{
    //DWORD hTex = Strata::ResourceManager::AddTexture( "GameSprites", 1024.0f, 1024.0f, "assets/spritesheet.png", "/switch/spritesheet.map" );

    Strata::HTEXTURE hSub = Strata::ResourceManager::GetTextureHandle( "GameSprites.VirusPink" );
    
    testSpr.ConfigureAnimation( 64.0f, 64.0f, 6 );
    testSpr.DefineAnimation( "Walk", 0, 0, 5, 0.3f );
    testSpr.PlayAnimation( "Walk", SPRITEANIM_LOOP_INFINITE );
    mAngle = 0.0f;

    testSpr.Initialize(256.0f, 118.0f, 128.0f, 128.0f, 2, hSub );

    bkgSpr.Initialize(0.0f, 0.0f, 640.0f, 360.0f, 0, Strata::ResourceManager::GetTextureHandle( "MenuBkg" ) );

   
}

VOID GameState_Test::HandleInput( VOID )
{

}

VOID GameState_Test::Update(FLOAT elapsedTime)
{
    //mPlayer.Update(elapsedTime);
    testSpr.UpdateAnimation(elapsedTime);

    const float rotationSpeed = 720.0f;   // 5 degrees per second
    mAngle += rotationSpeed * elapsedTime;
    if(mAngle >= 360.0f ) mAngle -= 360.0f;


    // Rotate sprite
    testSpr.Rotate( mAngle );
}

VOID GameState_Test::Draw( VOID )
{
    mSpriteRenderer.BeginDraw( Strata::SpriteRenderer::SORTTYPE::SORTTYPE_BACKTOFRONT );


    // for( int y = 0; y < 10; y++ )
    // {
    //     for( int x = 0; x < 20; x++ )
    //     {
    //         testSpr.Translate( x * 32.0f, y * 32.0f );
    //         testSpr.Draw( mSpriteRenderer );
    //     }
    //  }

    testSpr.Draw( mSpriteRenderer );

    bkgSpr.Draw(mSpriteRenderer);


    mSpriteRenderer.EndDraw();

    // Draw our sprites to the sprite renderer
    
    //mPlayer.Draw( mpSpriteRenderer );
    mSpriteRenderer.Render();
}