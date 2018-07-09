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

#ifndef _GAMESTATE_TEST_H_
#define _GAMESTATE_TEST_H_

#include <strata/strata.h>
#include "../game/game.h"
#include "../entities/player.h"

class GameState_Test : public Strata::GameState
{
public: 
    GameState_Test( Strata::Application * appContext );
    ~GameState_Test( VOID );

    VOID OnEnter( VOID );

    VOID HandleInput( VOID );
    VOID Update(FLOAT elapsedTime);
    VOID Draw( VOID );

private:

    FLOAT mAngle;

    Player mPlayer;
    Strata::Sprite testSpr, bkgSpr;
    Strata::SpriteFont fontSpr;
    Strata::SpriteRenderer mSpriteRenderer;

};

#endif