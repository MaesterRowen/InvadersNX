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

#include "player.h"
#include "enemycontroller.h"

Player::Player( VOID )
{
    mpInputManager = nullptr;

    isPressed = false;
    isBPressed = false;
    mHealthPoints = 0UL;
    mRocketsRemaining = 0UL;
    animLeft = false;
    animRight = false;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    canMove = true;
    canShield = false;
    canShoot = false;
    mCurrentSpeed = 0.0f;
    mMaximumSpeed = 300.0f;
    mAccelRate = 20.0f;
    mDecelRate = 15.0f;
}

VOID Player::Initialize( FLOAT x, FLOAT y, DWORD layerId, Strata::HTEXTURE hTexture )
{
    // Intialize player
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = 0.0f;
    mCurrentSpeed = 0.0f;
    mMaximumSpeed = 300.0f;
    mAccelRate = 20.0f;
    mDecelRate = 7.0f;
    mRocketsRemaining = 0UL;
    mHealthPoints = 0UL;

    // Shield properties
    mShieldPower = 100.0f;
    mShieldRechargeRate = 1.0f;
    mShieldStrength = 10.0f;
    mShieldDrainRate = 3.0f;
    mShieldActivateCost = 5.0f;
    mShieldActive = false;

    // Define our ship animations
    mShipSprite.Initialize( x - 16.0f, y - 16.0f, 32.0f, 32.0f, layerId, Strata::ResourceManager::GetTextureHandle( hTexture, "AuroraShip" ));
    mShipSprite.ConfigureAnimation( 64.0f, 64.0f, 1 );
    mShipSprite.DefineAnimation( "Idle", 0, 0, 0.0f );

    // Define our flame animations
    mEngineSprite.Initialize( x - 8.0f, y + 8.0f, 16.0f, 16.0f, layerId, Strata::ResourceManager::GetTextureHandle( hTexture, "EngineFlame" ));
    mEngineSprite.ConfigureAnimation( 16.0f, 16.0f, 2 );
    mEngineSprite.DefineAnimation( "Idle", 0, 1, 0.25f );

    // Define our shield animations
    mShieldSprite.Initialize( x - 32.0f, y - 48.0f, 64.0f, 64.0f, layerId, Strata::ResourceManager::GetTextureHandle( hTexture, "AuroraShield" ));
    mShieldSprite.ConfigureAnimation( 64.0f, 64.0f, 4 );
    mShieldSprite.DefineAnimation( "Activate", 0, 5, 10, 0.25f );
    mShieldSprite.DefineAnimation( "Deactivate", 11, 15, 0.125f );
    mShieldSprite.StopAnimation();

    // Set our default animations
    mShipSprite.PlayAnimation("Idle" );
    mEngineSprite.PlayAnimation( "Idle", SPRITEANIM_LOOP_INFINITE );
}
VOID Player::Draw( Strata::Renderer& renderer )
{
    // Draw the player
    mShipSprite.Draw( renderer );
    mEngineSprite.Draw( renderer );
    mShieldSprite.Draw( renderer );
}

VOID Player::Update( Strata::ParticleBatch * pParticles, std::vector<Projectile>& bullets, FLOAT elapsedTime )
{
    // Process input
    if (mpInputManager != nullptr )
    {
        if( canShoot == true && mpInputManager->IsKeyPressed(KEY_A) == TRUE )
        {
            // Play the sound associated with our laser fire
            mpAudioEngine->PlaySound("phaser");

            // Instantiate a laser projectile
            Strata::VECTOR3 dir = { 0.0f, -1.0f };
            Projectile bullet( mPosition.x, mPosition.y - 16.0f, dir, 750.0f, 3, PROJECTILE_OWNER_PLAYER);
            bullets.push_back( bullet );

            //std::cout << "Pressed A" << std::endl;
        }
        else if( canShoot == true && mpInputManager->IsKeyPressed(KEY_B) == TRUE )
        {
            // Only fire a rocket if we have rockets to fire
            if( mRocketsRemaining > 0 )
            {
                // Play the sound associated with our rocket fire
                mpAudioEngine->PlaySound("rocket");

                // Instantiate our rocket
                Strata::VECTOR3 dir = { 0.0f, -1.0f };
                Projectile bullet( mPosition.x, mPosition.y - 24.0f , dir, 600.0f, 0, PROJECTILE_OWNER_PLAYER, pParticles );
                bullets.push_back( bullet );

                // Decrement our number of rockets
                mRocketsRemaining--;
            }
        }

        // check thumbstick axis and update our player position
        if( canMove == true && mpInputManager->GetAxis( Strata::InputAxis_LeftThumbX ) < -0.1f )
        {
            if( mCurrentSpeed > -mMaximumSpeed )
            {
                mCurrentSpeed -= mAccelRate;
                if( mCurrentSpeed <= -mMaximumSpeed )
                    mCurrentSpeed = -mMaximumSpeed;
            }
            else 
            {
                mCurrentSpeed = -mMaximumSpeed;
            }
        }
        else if( canMove == true && mpInputManager->GetAxis( Strata::InputAxis_LeftThumbX ) > 0.1f )
        {
            if( mCurrentSpeed < mMaximumSpeed )
            {
                mCurrentSpeed += mAccelRate;
                if( mCurrentSpeed >= mMaximumSpeed )
                    mCurrentSpeed = mMaximumSpeed;
            }
            else 
            {
                mCurrentSpeed = mMaximumSpeed;
            }
        }
        else 
        {
            // Slow our ship down
            if( mCurrentSpeed < 0.0f )
            {
                mCurrentSpeed += mDecelRate;
                if( mCurrentSpeed > 0.0f ) mCurrentSpeed = 0.0f;
            }
            else if( mCurrentSpeed > 0.0f )
            {
                mCurrentSpeed -= mDecelRate;
                if( mCurrentSpeed < 0.0f ) mCurrentSpeed = 0.0f;
            }
            else 
            {
                mCurrentSpeed = 0.0f;
            }
        }

        // Next we check our triggers to use shields
        if( canShield == true && mpInputManager->IsKeyPressed( KEY_L) == TRUE )
        {
            if( mShieldPower >= mShieldActivateCost )
            {
                if( mShieldActive == false ) {
                    // Play the sound associated with shield
                    mpAudioEngine->PlaySound("shield");

                    mShieldSprite.PlayAnimation( "Activate", SPRITEANIM_LOOP_INFINITE );
                }

                mShieldActive = true;
                mShieldPower -= mShieldActivateCost;
                if( mShieldPower < 0.0f ) {
                    mShieldPower = 0.0f;
                    if( mShieldActive == true ) {
                        // Play the sound associated with shield
                        mpAudioEngine->PlaySound("shield");

                        mShieldSprite.PlayAnimation( "Deactivate", SPRITEANIM_LOOP_NONE );
                    }
                    mShieldActive = false;
                }
            }
        }
        else if( canShield == true && mShieldActive == true && mpInputManager->IsKeyDown( KEY_L ) == TRUE )
        {
            mShieldPower -= mShieldDrainRate * elapsedTime;
            if( mShieldPower < 0.0f ) {
                mShieldPower = 0.0f;
                // Play the sound associated with shield
                mpAudioEngine->PlaySound("shield");                
                mShieldSprite.PlayAnimation( "Deactivate", SPRITEANIM_LOOP_NONE );
                mShieldActive = false;
            }
        }
        else if( canShield == true )
        {
            // No shield button is down, so recharge shield
            if( mShieldActive == true ) {
                // Play the sound associated with shield
                mpAudioEngine->PlaySound("shield");                      
                mShieldSprite.PlayAnimation( "Deactivate", SPRITEANIM_LOOP_NONE );
            }
            mShieldActive = false;
            mShieldPower += mShieldRechargeRate * elapsedTime;
            if( mShieldPower > 100.0f ) mShieldPower = 100.0f;
        }

        // Transform position
        mPosition.x += mCurrentSpeed * elapsedTime;
    }

    // Clamp player to edge of screen
    if( mPosition.x < 32.0f )
    {
        mPosition.x = 32.0f;
        if( mCurrentSpeed < 0.0f ) mCurrentSpeed = 0.0f;
    }
    else if (mPosition.x > (640.0f - 32.0))
    {
        mPosition.x = 640.0f - 32.0f;
        if( mCurrentSpeed >0.0f ) mCurrentSpeed = 0.0f;
    }

    // Update all of our animations
    mShieldSprite.UpdateAnimation( elapsedTime );
    mShipSprite.UpdateAnimation( elapsedTime );
    mEngineSprite.UpdateAnimation( elapsedTime );

    // Update our sprite positions based on the new player position
    mShieldSprite.Translate( mPosition.x - 32.0f, mPosition.y - 48.0f );
    mShipSprite.Translate( mPosition.x - 16.0f, mPosition.y - 16.0f );
    mEngineSprite.Translate( mPosition.x - 8.0f, mPosition.y + 8.0f );   
}

// Collision check
VOID Player::CheckCollision( std::vector<Projectile>& bullets, std::vector<Explosion>& explosions, EnemyController& enemySwarm )
{
    for( DWORD i = 0; i < bullets.size();)
    {
        Projectile& bullet = bullets[i];

        // If this projectile is not from an enemy, then let's ignore it
        if(  bullet.GetOwner() != PROJECTILE_OWNER_ENEMY ) { i++; continue; }

        // If a projectile has collided with the player, then we need to
        // instantiate an explosion sprite with animation
        if( bullet.CollideWithPlayer( *this ) == true )
        {
            Strata::VECTOR3 position = { mPosition.x, mPosition.y + (mShieldActive == true ? -24.0f : 0.0f ), 0.0f };
            Strata::VECTOR3 direction = { 0.0f, 1.0f, 0.0f };

            Explosion boom( position, direction, bullet.GetProjectileSpeed() * (mShieldActive == true ? 0.0f : 0.25f), mShieldActive == true ? 1 : 0, 0.5f );
            explosions.push_back( boom );

            if( mShieldActive == true )
            {
                // Play the sound 
                mpAudioEngine->PlaySound("explosion_hit");      
            }
            else 
            {
                // Play sound
                mpAudioEngine->PlaySound("explosion_death");      
            }

            if( mShieldActive == true )
            {
                mShieldPower -= mShieldStrength;
                if( mShieldPower < 0.0f ) {
                    mShieldPower = 0.0f;
                    mShieldActive = false;
                }
            }
            else 
            {
                mHealthPoints -= bullet.GetDamage();
            }

            if( mHealthPoints <= 0 ) mHealthPoints = 0;

            // Remove the bullet from the list
            bullets[i] = bullets.back();
            bullets.pop_back();
        }
        else
        {
            i++;
        }
    }

    // Check if player collided with enemy swarm
    if( enemySwarm.CollideWithPlayer( *this ) == true )
    {
        Strata::VECTOR3 position = { mPosition.x, mPosition.y };
        Strata::VECTOR3 direction = { 0.0f, 1.0f, 0.0f };

        Explosion boom( position, direction, mCurrentSpeed * 0.25f, 0, 0.5f );
        explosions.push_back( boom );

        // Play sound
        mpAudioEngine->PlaySound("death");           

        mHealthPoints = 0;
    }
}