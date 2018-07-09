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

#ifndef _GAMESTATE_GAMEOVER_H_
#define _GAMESTATE_GAMEOVER_H_

#include <strata/strata.h>

class GameState_GameOver : public Strata::GameState
{
public:
    GameState_GameOver( Strata::Application* appContext );
    virtual ~GameState_GameOver( VOID )  { }

    VOID OnEnter( VOID );
    VOID OnExit( VOID );
    VOID OnSupsend( VOID ) { }
    VOID OnResume( VOID ) { }
    VOID HandleInput( VOID );
    VOID Update( FLOAT elapsedTime );
    VOID Draw( VOID );

private:
    enum STATE_TYPE {
        STATE_TYPE_FADEIN,
        STATE_TYPE_WAIT,
        STATE_TYPE_FADEOUT,
        STATE_TYPE_END
    };

    BOOL mHighScore;
    STATE_TYPE mCurrentState;
    FLOAT mAnimationTimer;

    Strata::Sprite mBlackSprite;
    Strata::InputManager mInputManager;
    Strata::SpriteRenderer mSpriteRenderer;

};

#endif