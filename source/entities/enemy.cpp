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

#include "enemy.h"

Enemy::Enemy( DWORD enemyID, FLOAT x, FLOAT y, FLOAT width, FLOAT height, DWORD enemyType, Strata::HTEXTURE hTexture ) :
    Strata::Sprite()
{
    // Store our enemy ID and type
    mEnemyID = enemyID;
    mEnemyType = enemyType;
    mEnraged = FALSE;

    DWORD initFrame = rand() % 6;

    Strata::HTEXTURE hSubTexture = 0UL;
    if( enemyType == 0 )
    {
        hSubTexture = Strata::ResourceManager::GetTextureHandle(hTexture, "VirusPink" );
        ConfigureAnimation( 64.0f, 64.0f, 6 );
        DefineAnimation( "Walk", initFrame, 0, 5, 0.6f );
        PlayAnimation( "Walk", SPRITEANIM_LOOP_INFINITE );
        mEnemyHealth = 10;
        mEnemyValue = 100;
        
    }
    else if( enemyType == 1 )
    {
        hSubTexture = Strata::ResourceManager::GetTextureHandle(hTexture, "VirusRed" );
        ConfigureAnimation( 64.0f, 64.0f, 6 );
        DefineAnimation( "Walk", initFrame, 0, 5, 0.6f );
        PlayAnimation( "Walk", SPRITEANIM_LOOP_INFINITE );
        mEnemyHealth = 10;
        mEnemyValue = 150;
    }
    else if( enemyType == 2 )
    {
        hSubTexture = Strata::ResourceManager::GetTextureHandle(hTexture, "Virus" );
        mEnemyHealth = 10;
    }
    else if( enemyType == 3 )
    {
        hSubTexture = Strata::ResourceManager::GetTextureHandle(hTexture, "BossOrb" );
        ConfigureAnimation( 64.0f, 64.0f, 6 );
        DefineAnimation( "Calm", 0, 15, 1.6 );
        DefineAnimation( "Angry", 16, 28, 0.65 );
        PlayAnimation( "Calm", SPRITEANIM_LOOP_INFINITE );
        mEnemyHealth = 50;
        mEnemyValue = 200;
    }

    // Initailize sprite
    Initialize( x, y, width, height, 2, hSubTexture );
}

VOID Enemy::Update( FLOAT deltaX, FLOAT deltaY, FLOAT elapsedTime )
{
    UpdateAnimation( elapsedTime );

    // Update our enemy position
    mPosition.x += deltaX;
    mPosition.y += deltaY;
}