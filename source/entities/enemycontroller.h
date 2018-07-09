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

#ifndef _ENEMYCONTROLLER_H_
#define _ENEMYCONTROLLER_H_

#include <strata/strata.h>
#include <vector>
#include <set>
#include <string>

#include "enemy.h"
#include "projectile.h"


#define SWARM_ENEMY_COUNT_X     11
#define SWARM_ENEMY_COUNT_Y     5

class Player;

typedef struct _LEVEL_DATA {
    DWORD Rows;
    DWORD Columns;
    std::vector<std::string> RowData;
} LEVEL_DATA, *PLEVEL_DATA;

class EnemyController 
{
public:
    EnemyController( VOID );
    virtual ~EnemyController( VOID ) { }

    VOID Initialize( FLOAT x, FLOAT y, DWORD waveNumber, Strata::HTEXTURE hTexture );
    VOID Draw( Strata::Renderer& renderer , Strata::SpriteFont * font = nullptr);
    VOID UpdateIntro( FLOAT elapsedTime );
    VOID Update( std::vector<Projectile>& bullets, FLOAT elapsedTime );

    VOID CheckCollision( Strata::AudioEngine* audioEngine, std::vector<Projectile>& bullets, std::vector<Explosion>& explosions);
    BOOL CollideWithPlayer( Player& player );

    FLOAT GetX(VOID) { return mPosition.x; }
    FLOAT GetY(VOID) { return mPosition.y; }

    const RECT& GetBounds( VOID ) { return mSwarmRect; }

    FLOAT GetTargetY( VOID ) { return mTargetY; }
    FLOAT GetTransitionY(VOID) { return mTransitionY; }
    FLOAT GetBottomStart( VOID ) { return mBottomStart; }
    
    VOID Clear( VOID ) { mActiveEnemies.clear(); mDeadEnemyIds.clear(); }
    BOOL IsStartReady( VOID ) { return mStartReady; }
    BOOL IsCleared( VOID ) { return mIsCleared; }

    VOID HookScoreCallback( std::function<VOID(DWORD, VOID*)> cb, VOID * context )
    {
        Score.Callback = cb;
        Score.Context = context;
    }

private:

    VOID LoadLayout( DWORD levelIndex, Strata::HTEXTURE hTexture );

    // Score handling
    struct {
        std::function<VOID(DWORD, VOID*)> Callback;
        VOID * Context;
    } Score;

    SDL_Rect mBounds;
    RECT mSwarmRect;

    // Variables
    BOOL mIsCleared;
    Strata::VECTOR3 mPosition;
    DWORD mEnemiesPerRow;
    DWORD mEnemiesPerCol;
    DWORD mEnemyCount;
    FLOAT mBottomStart;
    BOOL mStartReady;
    DWORD mCurrentWave;
    DWORD mCurrentStep;

    // Swarm Movement
    FLOAT mMaxHorizontalSpeed, mMaxVerticalSpeed;
    FLOAT mHorizontalSpeed, mVerticalSpeed;
    FLOAT mSpeedFactor;

    FLOAT mTargetY;
    FLOAT mTransitionY;
    BOOL mIsTransition;

    bool atLeft;
    FLOAT mMinimumRateOfFire;
    FLOAT mLastFireTime;
    FLOAT mFireTimer;

    Strata::VECTOR3 mCurrentDirection;
    FLOAT mMinSpeed, mMaxSpeed;
    DWORD mShootChance;
    Strata::HTEXTURE mAtlasTexture;

    std::vector<Enemy> mActiveEnemies;
    std::set<DWORD> mDeadEnemyIds;
    std::vector<LEVEL_DATA> mLevelList;

	bool CalculateAttack( Enemy& enemyObject, FLOAT elapsedTime );
	void HitLeftBound( FLOAT currentY, FLOAT elapsedTime );
	void HitRightBound(  FLOAT currentY, FLOAT elapsedTime );
	void HitTargetY( FLOAT elapsedTime );    
    VOID calculateBounds( RECT * pRect );
};

#endif