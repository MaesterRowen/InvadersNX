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

#ifndef _GAMESTATE_GAME_H_
#define _GAMESTATE_GAME_H_

#include <vector>
#include <strata/strata.h>
#include "../entities/projectile.h"
#include "../entities/player.h"
#include "../entities/enemycontroller.h"
#include "../entities/explosion.h"

class Game;

class GameState_Game : public Strata::GameState
{
public:
    GameState_Game( Strata::Application* appContext );
    virtual ~GameState_Game( VOID );

private:

    VOID OnEnter( VOID );
    VOID OnExit( VOID );
    VOID OnSuspend( VOID ) { }
    VOID OnResume( VOID ) { }
    VOID HandleInput( VOID );
    VOID Update( FLOAT elapsedTime );
    VOID Draw( VOID );    

private:
    enum STATE_TYPE {
        // States for main menu
        STATE_TYPE_GAME_FADEIN,
        STATE_TYPE_WAVE_INTRO,
        STATE_TYPE_WAVE_OUTRO,
        STATE_TYPE_WAVE_INPUT,
        STATE_TYPE_PAUSE_START,
        STATE_TYPE_PAUSE_INPUT,
        STATE_TYPE_PAUSE_RETURN,
        STATE_TYPE_GAME_FADEOUT
    };

    STATE_TYPE GetGameState( VOID ) { return mCurrentState; }

    BOOL CheckForDeath( VOID );
    BOOL CheckIsPaused( VOID ) 
    {
        return ( 
            mCurrentState == STATE_TYPE_PAUSE_START ||
            mCurrentState == STATE_TYPE_PAUSE_INPUT ||
            mCurrentState == STATE_TYPE_PAUSE_RETURN
        ) ? TRUE : FALSE;
    }

    // Game state Variables
    FLOAT mAnimationTimer;
    STATE_TYPE mCurrentState;
    FLOAT mModulateTimer;
    DWORD mCurrentScore;
    DWORD mCurrentWave;

    BYTE mWaveAlphaValue;
    DWORD mFocusOption;

    // Rendering Stuff
    Strata::SpriteRenderer mSpriteRenderer;

    // Engine Modules
    Strata::InputManager mInputManager;
    
    // Texture Handles
    Strata::HTEXTURE    mBkgTexture;
    Strata::HTEXTURE    mAtlasTexture;
    Strata::HTEXTURE    mGUITexture;
    Strata::HTEXTURE    mLogoTexture;
    Strata::HTEXTURE    mRocketIconTexture;

    // Game Objects
    Strata::Sprite      mBlackSprite;
    Strata::Sprite      mPausedSprite;
    Strata::Sprite      mLogoSprite;
    Strata::Sprite      mBkgSprite;
    Player              mPlayer;
    EnemyController     mSwarm;

    std::vector<Projectile> mProjectiles;
    std::vector<Explosion> mExplosions;

    // UI Sprites
    Strata::Sprite      mHealthBar;
    Strata::Sprite      mHealthBarBody;
    Strata::Sprite      mShieldBar;
    Strata::Sprite      mShieldBarBody;

    std::vector<Strata::Sprite> mUIRockets;

    // Particles
    Strata::ParticleBatch*  mpSmokeParticles;
    Strata::ParticleEngine   mParticleEngine;

    VOID updateBullets( FLOAT elapsedTime );
    VOID updateExplosions( FLOAT elapsedTime );
    VOID updatePowerups( FLOAT elapsedTime );
};

#endif