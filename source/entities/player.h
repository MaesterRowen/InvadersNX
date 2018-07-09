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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <strata/strata.h>
#include "projectile.h"
#include "explosion.h"

class Projectile;
class EnemyController;

class Player 
{
public:
    // Constructor / Destructor
    Player(VOID);
    ~Player(VOID) { }

    // Life Cycle Functions
    VOID Initialize( FLOAT x, FLOAT y, DWORD layerId, Strata::HTEXTURE hTexture );
    VOID Update( Strata::ParticleBatch * pParticles, std::vector<Projectile>& bullets, FLOAT elapsedTime );
    VOID Draw( Strata::Renderer& renderer );

    // Module Configuration
    VOID SetInputManager( Strata::InputManager * inputManager ) { mpInputManager = inputManager; }
    VOID SetAudioEngine( Strata::AudioEngine * audioEngine ) { mpAudioEngine = audioEngine; }

    // Player functionality
    VOID AddRocket( DWORD numRockets = 1UL ) { mRocketsRemaining += numRockets; }
    VOID AddHealthPoints( DWORD healthPoints ) { mHealthPoints += healthPoints; }
    DWORD GetHealthPoints( VOID ) { return mHealthPoints; }
    DWORD GetRocketsRemaining( VOID ) { return mRocketsRemaining; }
    DWORD GetShieldRemaining( VOID ) { return mShieldPower; }

    // Collision check
    VOID CheckCollision( std::vector<Projectile>& bullets, std::vector<Explosion>& explosions, EnemyController& enemySwarm );

    // Public Methods
    FLOAT GetX( VOID )  { return mPosition.x; }
    FLOAT GetY( VOID )  { return mPosition.y; }
    FLOAT GetWidth( VOID ) { return mShipSprite.GetWidth(); }
    FLOAT GetHeight( VOID ) { return mShipSprite.GetHeight(); }

    bool CanMove( VOID ) { return canMove; }
    bool CanShoot( VOID ) { return canShoot; }
    bool CanShield( VOID ) { return canShield; }

    VOID SetCanShield( bool val ) { canShield = val; }
    VOID SetCanShoot( bool val ) { canShoot = val; }
    VOID SetCanMove( bool val ) { canMove = val; }

    VOID ReleaseShield( VOID )
    {
        if( mShieldActive == true ) {
            mShieldSprite.PlayAnimation( "Deactivate", SPRITEANIM_LOOP_NONE );
        }
        mShieldActive = false;
    }

private:

    bool isPressed, isBPressed;
    bool animLeft, animRight;
    bool canShoot, canMove, canShield;

    FLOAT mShieldPower;
    FLOAT mShieldRechargeRate;
    FLOAT mShieldStrength;
    FLOAT mShieldDrainRate;
    FLOAT mShieldActivateCost;
    bool mShieldActive;

    // Manage Modules for controlling aspects of engine
    Strata::InputManager * mpInputManager;
    Strata::AudioEngine * mpAudioEngine;
    
    // Basic player properties
    Strata::VECTOR3 mPosition;
    int mHealthPoints;
    DWORD mRocketsRemaining;

    FLOAT mCurrentSpeed;
    FLOAT mMaximumSpeed;
    FLOAT mAccelRate;
    FLOAT mDecelRate;

    // Sprites that make up the player
    Strata::Sprite mShipSprite;
    Strata::Sprite mEngineSprite;
    Strata::Sprite mShieldSprite;
};

#endif