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

#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <strata/strata.h>
#include "enemy.h"
#include "player.h"

#define PROJECTILE_OWNER_PLAYER 100
#define PROJECTILE_OWNER_ENEMY  200


class Player;

class Projectile : public Strata::Sprite
{
public:
    Projectile( FLOAT x, FLOAT y, const Strata::VECTOR3& direction, float speed, DWORD type, DWORD owner, Strata::ParticleBatch* pSmokeEffect = nullptr, FLOAT angle = 0.0f);
    virtual ~Projectile( VOID ) { }

    bool Update( FLOAT left, FLOAT right, FLOAT top, FLOAT bottom, FLOAT elapsedTime );
    bool CollideWithEnemy( Enemy& enemy );
    bool CollideWithPlayer( Player& player );

    DWORD GetType( VOID ) { return mType; }
    DWORD GetOwner( VOID ) { return mOwner; }
    FLOAT GetProjectileSpeed( VOID ) { return mSpeed; }
    DWORD GetDamage( VOID ) { return (DWORD)mDamage; }

private:
    Strata::ParticleBatch *mpSmokeEffect;
    Strata::VECTOR3 mDirection;
    FLOAT mDamage;
    FLOAT mSpeed;
    DWORD mOwner;
    DWORD mType;
};


#endif