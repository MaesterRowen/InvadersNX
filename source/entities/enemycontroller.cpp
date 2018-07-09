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

#include <fstream>
#include <sstream>
#include "enemycontroller.h"
#include "player.h"

// Layout constants
#define ENEMY_WIDTH                 24.0f
#define ENEMY_HEIGHT                24.0f
#define ENEMY_SPACING_X             0.0f
#define ENEMY_SPACING_Y             0.0f
#define ENEMY_MARGIN_TOP            24.0f       // Equal to the enemy height
#define ENEMY_MARGIN_LEFT           24.0f
#define ENEMY_MARGIN_RIGHT          24.0f

#define ENEMY_STEP_DISTANCE         24.0f

// Boundaries used in calculating swarm movement
#define SWARM_BOUNDARY_LEFT         0.0f
#define SWARM_BOUNDARY_RIGHT        640.0f
#define SWARM_BOUNDARY_TOP          0.0f
#define SWARM_BOUNDARY_BOTTOM       360.0f

#ifdef NXDEBUG
    #define DRAW_BOUNDING_RECT            1
    #define DRAW_ENEMY_ID                 1
#endif


EnemyController::EnemyController( VOID )
{
    // Configure static variables
    mMaxHorizontalSpeed = 300.0f;
    mMaxVerticalSpeed = 120.0f;
    mMinSpeed = 100.0f;
    mMaxSpeed = 200.0f;
    mCurrentWave = 0;


    // Set initial variables
    mMinimumRateOfFire = 0.625f;
    mIsCleared = FALSE;
    mStartReady = FALSE;
    mFireTimer = 0.0f;
    mHorizontalSpeed = 60.0f;          // Speed of the swarms movement in the left/right direction
    mVerticalSpeed = 80.0f;            // Speed of the swarms movement in the down direction
    mSpeedFactor = 1.0f;
    mCurrentDirection = { -1.0f, 0.0f, 0.0f };
    mShootChance = 25;
    atLeft = false;
    mAtlasTexture = 0UL;
    mPosition = { 0.0f, 0.0f, 0.0f };
    mEnemiesPerRow = 0UL;
    mEnemiesPerCol = 0UL;
    mEnemyCount = 0UL;
    mCurrentStep = 0UL;

    // Cache our level data
    std::string levelFile = "assets/data/levels.dat";
    std::ifstream stream(levelFile);
    if( stream.is_open())
    {
        std::string line = "";
        for( DWORD i = 0; i < 10; i++ )
        {
            LEVEL_DATA levelData;           
            
            std::getline( stream, line );
            levelData.Columns = atoi( line.c_str() );

            std::getline( stream, line );
            levelData.Rows = atoi( line.c_str() );            

            // Loop through rows
            for( DWORD x = 0; x < levelData.Rows; x++ )
            {
                std::getline( stream, line );
                levelData.RowData.push_back( line );
            }

            // Add this entry
            mLevelList.push_back( levelData );
        }

        // Close the stream
        stream.close();

        // Seed our random number generator
        srand( (unsigned int)svcGetSystemTick() );
    }
}

VOID EnemyController::Initialize( FLOAT x, FLOAT y, DWORD waveNumber, Strata::HTEXTURE hTexture )
{
    // Reset our varaibles
    mMinimumRateOfFire = 0.625f;
    mIsCleared = FALSE;
    mStartReady = FALSE;
    mFireTimer = 0.0f;
    mLastFireTime = 0.0f;
    mHorizontalSpeed = 60.0f;          // Speed of the swarms movement in the left/right direction
    mVerticalSpeed = 80.0f;            // Speed of the swarms movement in the down direction
    mSpeedFactor = 1.0f;
    mCurrentDirection = { -1.0f, 0.0f, 0.0f };
    mShootChance = 25;
    atLeft = false;
    mEnemiesPerRow = 0UL;
    mEnemiesPerCol = 0UL;
    mEnemyCount = 0UL;
    mCurrentStep = 0UL;
    mIsTransition = FALSE;

    // Set speicfied variables
    mAtlasTexture = hTexture;
    mPosition = { x, y };

    // Set our wave number
    mCurrentWave = waveNumber;

    // Clear our enemy lists
    mActiveEnemies.clear();
    mDeadEnemyIds.clear();

    // Load the layout for this level
    LoadLayout( waveNumber - 1, hTexture );
}

VOID EnemyController::LoadLayout( DWORD levelIndex, Strata::HTEXTURE hTexture )
{
    if( levelIndex > mLevelList.size() ) levelIndex = mLevelList.size() - 1;
    LEVEL_DATA& data = mLevelList[levelIndex];

    std::cout << "Loading Level Layout:  " << levelIndex << std::endl;

    mMinimumRateOfFire /= 1.25f;
    mShootChance += 100;  
    mActiveEnemies.clear();
    mDeadEnemyIds.clear();
    mEnemiesPerRow = data.Columns;
    mEnemiesPerCol = data.Rows;
    mCurrentDirection = { 0.0f, 0.0f, 0.0f };
    mTargetY = 0.0f; 
    mIsTransition = FALSE;
    mTransitionY = 0.0f;

    FLOAT leftStart = mPosition.x - (mEnemiesPerRow * (ENEMY_WIDTH + ENEMY_SPACING_X) / 2.0f);
    FLOAT bottomStart = mPosition.y - ENEMY_HEIGHT;

    mBottomStart = (mEnemiesPerCol * (ENEMY_HEIGHT + ENEMY_SPACING_Y)) + ENEMY_MARGIN_TOP;

    // Randomize the direction
    unsigned int val = rand() % 1000 + 1;
    if( val > 500 ) mCurrentDirection.x = -1.0f;
    else mCurrentDirection.x = 1.0f;

    for( DWORD curRow = 0; curRow < data.RowData.size(); curRow++ )
    {
        // Retrieve the current row informaiton
        const std::string& row = data.RowData[curRow];
        std::cout << "Row " << curRow + 1 << ": " << row << std::endl;

        // Loop through each string and build our enemy swarm
        DWORD curCol = 0UL;
        for( auto it = row.begin(); it != row.end(); it++, curCol++ )
        {
            char obj = *it;

            DWORD enId = curCol + curRow * (row.length() - 1);
            FLOAT xPos = leftStart + curCol * ENEMY_WIDTH + curCol * ENEMY_SPACING_X;
            FLOAT yPos = bottomStart - (curRow * ENEMY_HEIGHT + curRow * ENEMY_SPACING_Y);

            if( obj == '1' )
            {
                // Create our enemy id
                DWORD enemyType = 0;

                // Create our enemy object
                Enemy enemy(enId, xPos, yPos, ENEMY_WIDTH, ENEMY_HEIGHT, enemyType, hTexture);
                mActiveEnemies.push_back( enemy );
            }
            else if( obj == '2' )
            {
                // Create our enemy id
                DWORD enemyType = 1;

                // Create our enemy object
                Enemy enemy(enId, xPos, yPos, ENEMY_WIDTH, ENEMY_HEIGHT, enemyType, hTexture);
                mActiveEnemies.push_back( enemy );
            }
            else if( obj == '3' )
            {
                // Create our enemy id
                DWORD enemyType = 2;

                // Create our enemy object
                Enemy enemy(enId, xPos, yPos, ENEMY_WIDTH, ENEMY_HEIGHT, enemyType, hTexture);
                mActiveEnemies.push_back( enemy );
            }
            else if( obj == 'B' )
            {
                // Create our enemy id
                DWORD enemyType = 3;

                // Create our enemy object
                Enemy enemy(enId, xPos, yPos, ENEMY_WIDTH, ENEMY_HEIGHT, enemyType, hTexture);
                mActiveEnemies.push_back( enemy );
            }
            else if( obj == '.' )
            {
                mDeadEnemyIds.insert( enId );
            }
            else 
            {
                // This is like going to be a CRLF or other character we don't support
                // so ignore it.
            }
        }
    }

    // Store our enemy count
    mEnemyCount = mActiveEnemies.size();
}
void EnemyController::HitLeftBound( FLOAT currentY, FLOAT elapsedTime )
{
	// We hit our left bound, so let's change our direction
	mCurrentDirection.x = 0.0f;
	mCurrentDirection.y = 1.0f;
    mTransitionY = currentY;
    mTargetY = currentY + ENEMY_STEP_DISTANCE;
	atLeft = true;
}
void EnemyController::HitRightBound( FLOAT currentY, FLOAT elapsedTime )
{
	mCurrentDirection.x = 0.0f;
	mCurrentDirection.y = 1.0f;
	
    // Set our transition Y and target y
    mTransitionY = currentY;
    mTargetY = currentY + ENEMY_STEP_DISTANCE;
	atLeft = false;
}
void EnemyController::HitTargetY( FLOAT elapsedTime )
{
	mCurrentDirection.y = 0.0f;
	if( atLeft ) mCurrentDirection.x = 1.0f;
	else mCurrentDirection.x = -1.0f;
}
bool EnemyController::CalculateAttack( Enemy& enemyObject, FLOAT elapsedTime )
{
	DWORD enemyId = enemyObject.GetEnemyID();
	bool canAttack = false;

	// First  check if the enemy was created on the very bottom row
	if( enemyId < mEnemiesPerRow ) {
		// If so, he can attack becuase there is nothing below him
		canAttack = true;
	}
	else 
	{
		// From the current enemy, let's loop through each row and 
		// see if there is an enemy in each space below
		while( enemyId > mEnemiesPerRow - 1 )
		{
			enemyId -= mEnemiesPerRow;
			if( mDeadEnemyIds.find( enemyId ) != mDeadEnemyIds.end() )
			{
				canAttack = true;
			} 
			else 
			{
				canAttack = false;
				break;
			}
		}
	}

	// See if we can attack
	if( canAttack == true && mFireTimer - mLastFireTime > mMinimumRateOfFire )
	{
		// We are ready to fire- so let's see if this enemy is going to fire
		unsigned int val = rand() % 10000 + 1;
		if( val <= mShootChance ) {
			return true;
		}
	}

	return false;
}
BOOL EnemyController::CollideWithPlayer( Player& player )
{
    // Frist let's loop through all our enemies
    for( DWORD idx = 0; idx < mActiveEnemies.size(); idx++ )
    {
        Enemy& enemy = mActiveEnemies[idx];

        // If the enemy is above the player then it cna't collide, and we can skip
        if( (enemy.GetY() + enemy.GetHeight()) < (player.GetY() + player.GetHeight() / 2.0f) ) continue;

        // If the enemy's x is anywhere within the players sprite, then it collides
        if( enemy.GetX() > player.GetX() - player.GetWidth() / 2.0f && 
            enemy.GetX() < player.GetX() + player.GetWidth() / 2.0f )
        {
            return TRUE;
        }
    }

    // No colliision with player
    return FALSE;
}
VOID EnemyController::CheckCollision( Strata::AudioEngine* audioEngine, std::vector<Projectile>& bullets, std::vector<Explosion>& explosions )
{
    // Loop through our projectiles
    for( DWORD i = 0; i < bullets.size(); i++ )
    {
        if( bullets[i].GetOwner() != PROJECTILE_OWNER_PLAYER ) continue;

        for( DWORD j = 0; j < mActiveEnemies.size(); )
        {
            if( bullets[i].CollideWithEnemy( mActiveEnemies[j]))
            {
                float x = mActiveEnemies[j].GetX();
                float y = mActiveEnemies[j].GetY();

                Projectile& bullet = bullets[i];
                Enemy& enemy = mActiveEnemies[j];

                if( enemy.ApplyDamage( bullet.GetDamage() ) == TRUE )
                {
                    mDeadEnemyIds.insert( mActiveEnemies[j].GetEnemyID());
                    //DWORD score = mActiveEnemies[j].GetEnemyValue();

                    mActiveEnemies[j] = mActiveEnemies.back();
                    mActiveEnemies.pop_back();

                   // mHorizontalSpeed += 0.5f;
                    if( mHorizontalSpeed >= mMaxHorizontalSpeed ) mHorizontalSpeed = mMaxHorizontalSpeed;

                    //mVerticalSpeed += 0.5f;
                    if( mVerticalSpeed >= mMaxVerticalSpeed ) mVerticalSpeed = mMaxVerticalSpeed;

                    Strata::VECTOR3 position= { x + ENEMY_WIDTH / 2.0f, y + ENEMY_HEIGHT / 2.0f, 0.0f };
                    Strata::VECTOR3 direction = { mCurrentDirection.x, -0.5f, 0.0f };

                    Explosion boom( position, direction, mHorizontalSpeed * 0.25f, 0, 0.5f );
                    explosions.push_back( boom );

                    // Play sound
                    if( audioEngine ) audioEngine->PlaySound("explosion_death");
                }
                else 
                {
                    Strata::VECTOR3 position= { x + ENEMY_WIDTH / 2.0f, y + ENEMY_HEIGHT / 2.0f, 0.0f };
                    Strata::VECTOR3 direction = { 0.0f, 0.0f, 0.0f };

                    Explosion boom( position, direction, mHorizontalSpeed * 0.25f, 1, 0.5f );
                    explosions.push_back( boom );       

                    // Play sound
                    if( audioEngine ) audioEngine->PlaySound("explosion_hit");
                }

                DWORD type = bullets[i].GetType();
                if( type != 0 ) {
                    bullets[i] = bullets.back();
                    bullets.pop_back();
                }

                i--;
                break;
            }
            else 
            {
                j++;
            }
        }
    }

    if( mActiveEnemies.size() == 1 )
    {
        Enemy& enemy = mActiveEnemies[0];
        if( enemy.GetEnemyType() == 3 ) 
        {
            if( enemy.SetEnraged( TRUE ) == TRUE ) {

            }
        }
    }

    if( mActiveEnemies.size() == 0 )
    {
        mCurrentWave++;
        if( mCurrentWave >= mLevelList.size() ) mCurrentWave = mLevelList.size() - 1;
        mIsCleared = TRUE;
        
        LoadLayout( mCurrentWave - 1, mAtlasTexture );
    }
    
}
VOID EnemyController::Draw( Strata::Renderer& renderer, Strata::SpriteFont * font )
{
    #ifdef DRAW_BOUNDING_RECT
        // Draw an outline around our swarm (for debugging)
        Strata::COLOR col; col.AsDWORD = MAKE_RGBA( 128, 0, 128, 255 );
        renderer.DrawRect( mBounds.x, mBounds.y, mBounds.x + mBounds.w, mBounds.y + mBounds.h, col, false );
    #endif

    // Loop through entire swarm and draw each enemy sprite
    for( DWORD i = 0; i < mActiveEnemies.size(); i++ )
    {
        Enemy& enemy = mActiveEnemies[i];

        #ifdef DRAW_ENEMY_ID
            // Draw our ID on each enemy
            if( font ) {
                font->SetScale( 0.1875f );
                font->SetColor( 255, 255, 255, 255 );            
                font->DrawText( renderer, enemy.GetX() + enemy.GetWidth() / 2.0f, enemy.GetY() + enemy.GetHeight() / 2.0f, 
                    SPRITEFONT_ALIGN_CENTER, 15, "%d", enemy.GetEnemyID() );
            }
        #endif


        #ifdef DRAW_BOUNDING_RECT
            // Draw our bounding box
            Strata::COLOR col; col.AsDWORD = MAKE_RGBA( 0, 255, 0, 255 );
            renderer.DrawRect( enemy.GetX(), enemy.GetY(), enemy.GetX() + enemy.GetWidth(), enemy.GetY() + enemy.GetHeight(), col, false );
        #endif

        enemy.Draw( renderer );
    }
}
VOID EnemyController::calculateBounds( RECT * pRect )
{
    if( pRect == nullptr ) return;

    // Get containing RECT of the swarm
    float leftMost = SWARM_BOUNDARY_RIGHT, rightMost = SWARM_BOUNDARY_LEFT;
    float bottomMost = SWARM_BOUNDARY_TOP, topMost = SWARM_BOUNDARY_BOTTOM;
    for( DWORD i = 0; i < mActiveEnemies.size(); i++ )
    {
        if( mActiveEnemies[i].GetX() <= leftMost ) leftMost = mActiveEnemies[i].GetX();
        if( mActiveEnemies[i].GetX() >= rightMost ) rightMost = mActiveEnemies[i].GetX();
        if( mActiveEnemies[i].GetY() >= bottomMost ) bottomMost = mActiveEnemies[i].GetY();
        if( mActiveEnemies[i].GetY() <= topMost ) topMost = mActiveEnemies[i].GetY();
    }

    // Fill out RECT info
    pRect->left = leftMost;
    pRect->right = rightMost + ENEMY_WIDTH;
    pRect->top = topMost;
    pRect->bottom = bottomMost + ENEMY_HEIGHT;

    mSwarmRect = *pRect;
}
VOID EnemyController::UpdateIntro( FLOAT elapsedTime )
{
    mIsCleared = FALSE;

    // Calculate the bounds of the swarm
    RECT boundingRect = { 0 };
    calculateBounds( &boundingRect );
    
    // Calculate how much space we have to move before hitting bound
    float bottomSpace = mBottomStart - boundingRect.bottom;

    // Our direction vector has been updated, so let's detemrine how much to move
    float distanceToMoveY = mVerticalSpeed * elapsedTime * mSpeedFactor;

    // Clamp our movement to one of our Y-bounds
    if( distanceToMoveY > bottomSpace ) distanceToMoveY = bottomSpace;

    // Loop through each active enemy in the swarm and update
    for( DWORD i = 0; i < mActiveEnemies.size(); i++ )
    {
        mActiveEnemies[i].Update( 0.0f, distanceToMoveY, elapsedTime );
    }

    // Flag our start phase
    if( boundingRect.bottom >= mBottomStart ) mStartReady = TRUE;
    else mStartReady = FALSE;
}
VOID EnemyController::Update( std::vector< Projectile>& bullets, FLOAT elapsedTime )
{
    // Calculate the bounds of the swarm
    RECT boundingRect = { 0 };
    calculateBounds( &boundingRect );

    // Store the swarm bounds
    mBounds.x = boundingRect.left; mBounds.w = boundingRect.right - boundingRect.left;
    mBounds.y = boundingRect.top; mBounds.h = boundingRect.bottom - boundingRect.top;

    // Determine if we hit a boundary
    if( boundingRect.left <= (SWARM_BOUNDARY_LEFT + ENEMY_MARGIN_LEFT) && mCurrentDirection.y == 0.0f ) 
    { 
        HitLeftBound( boundingRect.bottom, elapsedTime );
        mIsTransition = TRUE;
    }
    else if( boundingRect.right >= (SWARM_BOUNDARY_RIGHT - ENEMY_MARGIN_RIGHT) && mCurrentDirection.y == 0.0f ) 
    { 
        HitRightBound( boundingRect.bottom, elapsedTime ); 
        mIsTransition = TRUE;
    }
    else if( mTransitionY >= mTargetY && mCurrentDirection.x == 0.0f ) 
    {
        HitTargetY( elapsedTime );
        mIsTransition = FALSE;
    }

    // Calculate how much space we have to move before hitting a bount
    float leftSpace = boundingRect.left - (SWARM_BOUNDARY_LEFT + ENEMY_MARGIN_LEFT);
    float rightSpace = (SWARM_BOUNDARY_RIGHT - ENEMY_MARGIN_RIGHT) - boundingRect.right;

    float bottomSpace = mIsTransition == TRUE ? mTargetY - mTransitionY : mTargetY - boundingRect.bottom;

    // Out direction vector has been updated, so let's determine how much to move
    float distanceToMoveX = mHorizontalSpeed * elapsedTime * mSpeedFactor;
    float distanceToMoveY = mVerticalSpeed * elapsedTime * mSpeedFactor;

    // Clamp movement to one of our X-bounds
    if( mCurrentDirection.x < 0.0f && distanceToMoveX > leftSpace ) distanceToMoveX = leftSpace;
    else if( mCurrentDirection.x > 0.0f && distanceToMoveX > rightSpace ) distanceToMoveX = rightSpace;

    // Clamp our movement to one of our Y-bounds
    if( distanceToMoveY > bottomSpace ) distanceToMoveY = bottomSpace;

    // Now apply our direction to our distance
    distanceToMoveX *= mCurrentDirection.x;
    distanceToMoveY *= mCurrentDirection.y;

    // Increment our mTranstionY
    if( mIsTransition == TRUE )
    {
        mTransitionY += distanceToMoveY;
    }

    // Incrrement our fire timer
    mFireTimer += elapsedTime;

    // Loop throgh each active enemy in swarm and update
    for( DWORD idx = 0; idx < mActiveEnemies.size(); idx++ )
    {
        Enemy& enemy = mActiveEnemies[idx];

        // Calculate chance to attack and attack if ready
        if( CalculateAttack( enemy, elapsedTime ) == true )
        {
            // Reset our last fire time
            mLastFireTime = mFireTimer;

            DWORD enemyType = enemy.GetEnemyType();
            DWORD projectileType = (enemyType % 2) + 1;

            // If the enemy is in rage mode, then we need to fire some harder guns
            if( enemy.IsEnraged() == TRUE )
            {
                // Determine the origin
                float centerX = enemy.GetX(); 
                float centerY = enemy.GetY();

                // Number of projectiles to shoot
                for( DWORD x = 2; x < 7; x++ )
                {
                    float oldX = 1.0f, oldY = 0.0f;
                    float newX = oldX * cos( (0.125f * x) * STRATA_PI ) - oldY * sin( (0.125f * x) * STRATA_PI );
                    float newY = oldX * sin( (0.125f * x) * STRATA_PI ) + oldY * cos( (0.125f * x) * STRATA_PI );

                    float degrees = 90.0f + (0.125f * x) * 180.0f;

                    const Strata::VECTOR3 direction = { newX, newY, 0.0f };
                    Projectile laser( centerX, centerY, direction, 500.0f, projectileType, PROJECTILE_OWNER_ENEMY, NULL, degrees);
                    bullets.push_back( laser );
                }
            }
            else 
            {
                const Strata::VECTOR3 direction = { 0.0f, 1.0f, 0.0f };
                Projectile laser( enemy.GetX() + enemy.GetWidth() / 2.0f, enemy.GetY() + enemy.GetHeight(), direction, 750.0f, projectileType, PROJECTILE_OWNER_ENEMY );
                bullets.push_back( laser );
            }
        }
        
        // Update hte enemies position
        enemy.Update(distanceToMoveX, distanceToMoveY, elapsedTime);
    }
}

