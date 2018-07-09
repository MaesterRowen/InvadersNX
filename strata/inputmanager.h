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

#ifndef _STRATA_INPUTMANAGER_H_
#define _STRATA_INPUTMANAGER_H_

#include <switch.h>
#include "types.h"

namespace Strata 
{
    typedef enum {
        InputAxis_LeftThumbX,
        InputAxis_LeftThumbY,
        InputAxis_RightThumbX,
        InputAxis_RightThumbY,
        InputAxis_Count
    } InputAxis;


    class InputManager 
    {
    public:
        InputManager( VOID );
        ~InputManager( VOID );
        VOID Initialize( HidControllerID controllerId = CONTROLLER_P1_AUTO );

        VOID Poll( VOID );

        BOOL IsKeyDown( HidControllerKeys keyId );
        BOOL IsKeyPressed( HidControllerKeys keyId );

        FLOAT GetAxis( InputAxis axisId );

    private:

        QWORD mButtonState;
        QWORD mPreviousButtonState;

        HidControllerID mControllerId;

    };
}

#endif