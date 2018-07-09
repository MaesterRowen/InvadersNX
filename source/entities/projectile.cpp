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
#include "projectile.h"

Projectile::Projectile( FLOAT x, FLOAT y, const Strata::VECTOR3& direction, float speed, DWORD type, DWORD owner, Strata::ParticleBatch* pSmokeEffect /*= nullptr*/, FLOAT angle /*= 0.0f*/ ) :
    Strata::Sprite()
{
    // Find texture based on type
    Strata::HTEXTURE hTexture = 0UL;
    if( type == 1 ) {
        hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.Laser1" );
        mDamage = 10;
    } else if( type == 2 ) {
        hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.Laser2" );
        mDamage = 20;       
    } else if( type == 3 ) {
        hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.Laser3" );
        mDamage = 10;       
    } else {
        hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.Rocket" );
        mDamage = 50;           
    }

    // Initlaize the projectile
    Initialize( x - 4.0f, y - 8.0f, 8.0f, 16.0f, 4, hTexture );
    ConfigureAnimation( 8.0f, 16.0f, 2 );
    DefineAnimation( "Fire", 0, 1, 0.125f );

    // Play the animation
    PlayAnimation( "Fire", SPRITEANIM_LOOP_INFINITE );

    // configure the starting speed and direction
    mpSmokeEffect = pSmokeEffect;
    mDirection = direction;
    mSpeed = speed;
    mOwner = owner;
    mType = type;
    mRotationAngle = angle;
}

bool Projectile::Update( float left, float right, float top, float bottom, FLOAT elapsedTime )
{
    // Update the bullet's location
    mPosition.x += mDirection.x * mSpeed * elapsedTime;
    mPosition.y += mDirection.y * mSpeed * elapsedTime;

    // check for collision with screen boundary
    if( mPosition.x <= left ) return true;
    if( mPosition.x >= right ) return true;
    if( mPosition.y <= top ) return true;
    if( mPosition.y >= bottom ) return true;

    // Update the animation
    UpdateAnimation(elapsedTime );

    // Process particles
    if( mpSmokeEffect ) {
        Strata::VECTOR3 dir = { 0.0f, 0.0f, 0.0f };
        Strata::COLOR col; col.AsDWORD = MAKE_RGBA( 255, 255, 255, 255 );

        for( DWORD i = 0; i < 3; i++ )
        {
            Strata::VECTOR3 pos1 = { mPosition.x + GetWidth() / 2.0f, mPosition.y + GetHeight() / 2.0f + 12.0f + 5.0f * i };
            mpSmokeEffect->AddParticle( pos1, dir, 0.75f, col, 24.0f );
        }   
    }

    // No collissions
    return false;
}
bool Projectile::CollideWithEnemy( Enemy& enemy )
{
    float x = enemy.GetX(); float y= enemy.GetY();
    float w = enemy.GetWidth();

    const float minDistance = w / 2.0f + GetWidth() / 2.0f;

    // Create our float for the center of the player
    const Strata::VECTOR3 enemyCenterF = { x + w / 2.0f, y + w / 2.0f };

    // Create our float for the center of our projectile
    const Strata::VECTOR3 projectileCenterF = { mPosition.x + GetWidth() / 2.0f, mPosition.y + GetHeight() / 2.0f };

    // Calculate the distnace between the center of the player and the current object
    float distance = sqrtf( powf(projectileCenterF.x - enemyCenterF.x, 2.0f) + powf(projectileCenterF.y - enemyCenterF.y, 2.0f));
    return (minDistance - distance) > 0.0f  ? true : false;
}
bool Projectile::CollideWithPlayer( Player& player )
{
    float x = player.GetX(); float y= player.GetY();
    float w = player.GetWidth() - 8.0f;

    const float minDistance = w / 2.0f + GetWidth() / 2.0f;

    // Create our float for the center of the player
    const Strata::VECTOR3 playerCenterF = { x, y };

    // Create our float for the center of our projectile
    const Strata::VECTOR3 projectileCenterF = { mPosition.x + GetWidth() / 2.0f, mPosition.y + GetHeight() / 2.0f };

    // Calculate the distnace between the center of the player and the current object
    float distance = sqrtf( powf(projectileCenterF.x - playerCenterF.x, 2.0f) + powf(projectileCenterF.y - playerCenterF.y, 2.0f));
    return (minDistance - distance) > 0.0f  ? true : false;
}