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

#include <iostream>
#include <switch.h>
#include <SDL2/SDL_joystick.h>
#include "inputmanager.h"

#define JOYSTICK_MAXVALUE   32767.0f
#define LSTICK_RANGE_LOW    0.1f
#define LSTICK_RANGE_HIGH   0.75f
#define RSTICK_RANGE_LOW    0.1f
#define RSTICK_RANGE_HIGH   0.75f

namespace Strata 
{
    InputManager::InputManager( VOID )
    {
        mButtonState = 0ULL;
        mPreviousButtonState = 0ULL;
        mControllerId = CONTROLLER_P1_AUTO;
    }

    InputManager::~InputManager( VOID )
    {

    }

    VOID InputManager::Initialize( HidControllerID controllerId /*= CONTROLLER_P1_AUTO*/ )
    {
        // Clamp controller id
        if( controllerId > CONTROLLER_P1_AUTO ) {
            controllerId = CONTROLLER_P1_AUTO;
        }

        // Set our controller id
        mControllerId = controllerId;
    }

    VOID InputManager::Poll( VOID )
    {
        // Scan for input 
        hidScanInput();

        // Set the previous state = to the last state obtained
        mPreviousButtonState = mButtonState;

        // Determine the current state of the buttons
        mButtonState = hidKeysHeld( mControllerId );

        // Process our left analog stick values
        JoystickPosition jsLeft, jsRight;
        hidJoystickRead( &jsLeft, mControllerId, JOYSTICK_LEFT );
        hidJoystickRead( &jsRight, mControllerId, JOYSTICK_RIGHT );

        float ldx = jsLeft.dx / JOYSTICK_MAXVALUE, ldy = jsLeft.dy / JOYSTICK_MAXVALUE;
        float rdx = jsRight.dx / JOYSTICK_MAXVALUE, rdy = jsRight.dy / JOYSTICK_MAXVALUE;

        // Process the up and down on the left joycon
        static bool ls_up_held = false;
        static bool ls_down_held = false;
        if( ldy > LSTICK_RANGE_HIGH && ls_up_held == false )
        {
            ls_up_held = true;
            mButtonState |= KEY_LSTICK_UP;
        }
        else if( ldy < LSTICK_RANGE_LOW && ls_up_held == true )
        {
            ls_up_held = false;
        }
        if( ldy < -LSTICK_RANGE_HIGH && ls_down_held == false )
        {
            ls_down_held = true;
            mButtonState |= KEY_LSTICK_DOWN;
        }
        else if( ldy > -LSTICK_RANGE_LOW && ls_down_held == true )
        {
            ls_down_held = false;
        }

        // Process the left and right on the left joycon
        static bool ls_left_held = false;
        static bool ls_right_held = false;
        if( ldx > LSTICK_RANGE_HIGH && ls_left_held == false )
        {
            ls_left_held = true;
            mButtonState |= KEY_LSTICK_LEFT;
        }
        else if( ldx < LSTICK_RANGE_LOW && ls_left_held == true )
        {
            ls_left_held = false;
        }
        if( ldx < -LSTICK_RANGE_HIGH && ls_right_held == false )
        {
            ls_right_held = true;
            mButtonState |= KEY_LSTICK_RIGHT;
        }
        else if( ldx > -LSTICK_RANGE_LOW && ls_right_held == true )
        {
            ls_right_held = false;
        }        

        // Process the up and down on the left joycon
        static bool rs_up_held = false;
        static bool rs_down_held = false;
        if( rdy > RSTICK_RANGE_HIGH && rs_up_held == false )
        {
            rs_up_held = true;
            mButtonState |= KEY_RSTICK_UP;
        }
        else if( rdy < RSTICK_RANGE_LOW && rs_up_held == true )
        {
            rs_up_held = false;
        }
        if( rdy < -RSTICK_RANGE_HIGH && rs_down_held == false )
        {
            rs_down_held = true;
            mButtonState |= KEY_RSTICK_DOWN;
        }
        else if( rdy > -RSTICK_RANGE_LOW && rs_down_held == true )
        {
            rs_down_held = false;
        }

        // Process the left and right on the left joycon
        static bool rs_left_held = false;
        static bool rs_right_held = false;
        if( rdx > RSTICK_RANGE_HIGH && rs_left_held == false )
        {
            rs_left_held = true;
            mButtonState |= KEY_RSTICK_LEFT;
        }
        else if( rdx < RSTICK_RANGE_LOW && rs_left_held == true )
        {
            rs_left_held = false;
        }
        if( rdx < -RSTICK_RANGE_HIGH && rs_right_held == false )
        {
            rs_right_held = true;
            mButtonState |= KEY_RSTICK_RIGHT;
        }
        else if( rdx > -RSTICK_RANGE_LOW && rs_right_held == true )
        {
            rs_right_held = false;
        }                
    }

    FLOAT InputManager::GetAxis( InputAxis axisId )
    {
        if( axisId == InputAxis_LeftThumbX || axisId == InputAxis_LeftThumbY ) 
        {
            JoystickPosition p1joy;
            hidJoystickRead(&p1joy, mControllerId, JOYSTICK_LEFT );

            if( axisId == InputAxis::InputAxis_LeftThumbX ) return p1joy.dx / JOYSTICK_MAXVALUE;
            if( axisId == InputAxis::InputAxis_LeftThumbY ) return p1joy.dy / JOYSTICK_MAXVALUE;
        }
        else if( axisId == InputAxis_RightThumbX || axisId == InputAxis_RightThumbY )
        {
            JoystickPosition p1joy;
            hidJoystickRead(&p1joy, mControllerId, JOYSTICK_RIGHT );

            if( axisId == InputAxis::InputAxis_RightThumbX ) return p1joy.dx / JOYSTICK_MAXVALUE;
            if( axisId == InputAxis::InputAxis_RightThumbY ) return p1joy.dy / JOYSTICK_MAXVALUE;
        }

        // Unreconginzed Axis
        return 0.0f;
    }

    BOOL InputManager::IsKeyDown( HidControllerKeys keyId )
    {
        return (( mButtonState & keyId ) == keyId ) ? TRUE : FALSE;
    }

    BOOL InputManager::IsKeyPressed( HidControllerKeys keyId )
    {
        if((mButtonState & keyId) == keyId && (mPreviousButtonState & keyId) == 0 )
        {
            return TRUE;
        }
        return FALSE;
    }
}