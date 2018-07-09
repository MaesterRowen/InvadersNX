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

#include "explosion.h"

Explosion::Explosion( const Strata::VECTOR3& position, const Strata::VECTOR3& direction, float speed, float type, float timeToLive )
    : Strata::Sprite()
{
    if( type == 0 )
    {
        Strata::HTEXTURE hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.LargeExplosion" );

        // Intialize Object
        Initialize( position.x - 32.0f, position.y - 32.0f, 64.0f, 64.0f, 5, hTexture );
        ConfigureAnimation( 192.0f, 192.0f, 5 );
        DefineAnimation( "Boom", 0, 6, 0.5f );
    }
    else if( type == 1 )
    {
        Strata::HTEXTURE hTexture = Strata::ResourceManager::GetTextureHandle( "GameSprites.SmallExplosion" );

        // Intialize Object
        Initialize( position.x - 24.0f, position.y - 24.0f, 48.0f, 48.0f, 5, hTexture );
        ConfigureAnimation( 125.0f, 125.0f, 3 );
        DefineAnimation( "Boom", 0, 5, 0.35f );
    }

    PlayAnimation( "Boom", 0UL );

    // Set the speed and direction
    mDirection = direction;
    mSpeed = speed;
    mTimeToLive = timeToLive;
}

Explosion::~Explosion()
{
    StopAnimation();
}

BOOL Explosion::Update( FLOAT elapsedTime )
{
    UpdateAnimation( elapsedTime );

    mPosition.x += mDirection.x * mSpeed * elapsedTime;
    mPosition.y += mDirection.y * mSpeed * elapsedTime;

    mTimeToLive -= elapsedTime;
    if( mTimeToLive <= 0.0f )
    {
        return TRUE;
    }

    return FALSE;
}