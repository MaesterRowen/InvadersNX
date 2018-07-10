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

#ifndef _GAME_H_
#define _GAME_H_
#include <strata/strata.h>

#define SCORE_ENTRY_MAX 10

class Game : public Strata::Application
{
private:
    Strata::GameDirector mDirector;

    Strata::SpriteFont mFont;
    Strata::SpriteRenderer mSpriteRenderer;
    Strata::InputManager mInputManager;
    Strata::AudioEngine mAudioEngine;

	VOID ParseHighScoreData( VOID );    

public:
    BOOL Initialize( VOID );
    DWORD Update( FLOAT elapsedTime );
    DWORD Render( VOID );

    ~Game( VOID );

	typedef struct _SCORE_ENTRY { 
		char Name[32];
		DWORD Score;
		DWORD Wave;
	} SCORE_ENTRY, *pSCORE_ENTRY;

	SCORE_ENTRY mScoreEntries[SCORE_ENTRY_MAX];
	DWORD mScoreEntryCount;

    VOID QuitApplication( VOID ) {
        mRunning = FALSE;
    }

    Strata::AudioEngine& GetAudioEngine() { return mAudioEngine; }
    Strata::InputManager& GetInputManager() { return mInputManager; }
    Strata::GameDirector& GetStateManager() { return mDirector; }
    Strata::SpriteFont& GetFont() { return mFont; }
};
#endif