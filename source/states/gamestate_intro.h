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

#ifndef _GAMESTATE_INTRO_H_
#define _GAMESTATE_INTRO_H_

#include <strata/strata.h>

class GameState_Intro : public Strata::GameState
{
public:
    enum STATE_TYPE {
        STATE_TYPE_FADEIN,
        STATE_TYPE_WAIT,
        STATE_TYPE_FADEOUT,
        STATE_TYPE_END
    };

    GameState_Intro( Strata::Application* appContext );
    virtual ~GameState_Intro( VOID );

    VOID OnEnter( VOID );
    VOID OnExit( VOID );
    VOID OnSuspend( VOID );
    VOID OnResume( VOID );
    VOID HandleInput( VOID );
    VOID Update( FLOAT elapsedTime );
    VOID Draw( VOID );

private:
    STATE_TYPE mCurrentState;
    FLOAT mAnimationTimer;
    bool mIsLoaded;

    Strata::Sprite mLogoSprite;
    Strata::InputManager mInputManager;
    Strata::SpriteRenderer mSpriteRenderer;
    Strata::SpriteFont mFont;

};

#endif