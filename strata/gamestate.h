// Strata Framework
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

#ifndef _STRATA_GAMESTATE_H_
#define _STRATA_GAMESTATE_H_

#include "types.h"
#include "application.h"

namespace Strata
{
    class GameState
    {
    public:
        virtual void OnEnter( VOID ) {}
        virtual void OnExit( VOID ) {}
        virtual void OnSuspend( VOID ) {}
        virtual void OnResume( VOID ) {}

        virtual void HandleInput( VOID ) = 0;
        virtual void Update(FLOAT elapsedTime ) = 0;
        virtual void Draw( VOID );

        virtual ~GameState() { }        

    protected:

        Application * mpAppContext;
        GameState( Application * appContext ) : 
            mpAppContext(appContext) {
            // Constructor
        }
    };
}

#endif