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

#ifndef _GAMESTATE_MAINMENU_H_
#define _GAMESTATE_MAINMENU_H_

#include <strata/strata.h>

class GameState_MainMenu : public Strata::GameState
{
public:
    enum STATE_TYPE {
        // States for main menu
        STATE_TYPE_FADEIN,
        STATE_TYPE_INPUT,
        STATE_TYPE_FADEOUT,
        STATE_TYPE_FADEIN_WAIT,
        STATE_TYPE_FADEOUT_WAIT,
        STATE_TYPE_END
    };

    enum MENU_TYPE {
        // Types of menu
        MENU_TYPE_NONE,
        MENU_TYPE_MAIN,
        //MENU_TYPE_HIGHSCORE,
        MENU_TYPE_CONTROLS,
        MENU_TYPE_GAME
    };

    GameState_MainMenu( Strata::Application* appContext );

    VOID OnEnter( VOID );
    VOID OnExit( VOID );
    VOID OnSuspend( VOID );
    VOID OnResume( VOID );

    VOID HandleInput( VOID );
    VOID Update(FLOAT elapsedTime );
    VOID Draw( VOID );

private:
    Strata::COLOR ModulateFocus( VOID );

    FLOAT mModulateTimer;
    DWORD mLastMainFocus;
    
    STATE_TYPE mCurrentState;
    MENU_TYPE mCurrentMenu;
    MENU_TYPE mTargetMenu;

    FLOAT mAnimationTimer;
    FLOAT mFadeOutTime;
    FLOAT mFadeInTime;

    DWORD mFocusOption;

    Strata::SpriteRenderer mSpriteRenderer;

    // Sprites used for menu items
    Strata::Sprite mControlsSprite;
    Strata::Sprite mBlackSprite;
    Strata::Sprite mLogoSprite;
    Strata::Sprite mBkgSprite;
    Strata::Sprite mPanelSprite;
    Strata::Sprite mCaptionSprite;
};

#endif