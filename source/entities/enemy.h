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

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <strata/strata.h>

class Enemy : public Strata::Sprite
{
public:
    Enemy( DWORD enemyID, FLOAT x, FLOAT y, FLOAT width, FLOAT height, DWORD enemyType, Strata::HTEXTURE hTexture );
    virtual ~Enemy( VOID ) { }

    VOID Update( FLOAT deltaX, FLOAT deltaY, FLOAT elapsedTime );
    DWORD GetEnemyID( VOID ) { return mEnemyID; }
    DWORD GetEnemyType( VOID ) { return mEnemyType; }
    DWORD GetEnemyHealth( VOID ) { return mEnemyHealth; }
    BOOL ApplyDamage( DWORD dmg ) { mEnemyHealth -= dmg; if( mEnemyHealth <= 0 ) { mEnemyHealth = 0; return TRUE; } return FALSE; }
    BOOL IsEnraged( VOID ) { return mEnraged; }
    DWORD GetEnemyValue( VOID ) { return mEnemyValue; }
    BOOL SetEnraged( BOOL val ) 
    {
        if( mEnraged == FALSE && val == TRUE && mEnemyType == 3 )
        {
            mEnraged = TRUE;
            mEnemyValue *= 5;
            PlayAnimation( "Angry", SPRITEANIM_LOOP_INFINITE );
            return TRUE;
        }
        else if( mEnraged == TRUE && val == FALSE && mEnemyType == 3 )
        {
            mEnemyValue /= 5;
            PlayAnimation( "Calm", SPRITEANIM_LOOP_INFINITE );
            return TRUE;
        }

        return FALSE;
    }

private:
    int mEnemyHealth;
    DWORD mEnemyID;
    DWORD mEnemyType;
    BOOL mEnraged;
    DWORD mEnemyValue;
};

#endif