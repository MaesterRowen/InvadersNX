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

#include "gamedirector.h"

namespace Strata
{
    GameDirector::GameDirector( VOID ) 
    {
        mInFrame = false;
        mStateChanged = false;
    }

    GameDirector::~GameDirector( VOID )
    {
        // Free all of our game states
        for( auto it = mStates.begin(); it != mStates.end(); ++it )
        {
            GameState * pState = it->second;
            if( pState == nullptr ) continue;

            delete pState;
            pState = nullptr;
        }

        // Clear the map
        mStates.clear();
    }

    VOID GameDirector::Add( const std::string& stateName, GameState* pState )
    {
        mStates.insert( std::make_pair( stateName, pState ) );
    }

    VOID GameDirector::PushState( const std::string& stateName )
    {
        auto pState = mStates.find( stateName );
        if( pState != mStates.end() )
        {
            // First suspend our currently running state
            if( mStack.size() > 0 ) mStack.back()->OnSuspend();

            // Next push the new state onto the stack
            mStack.push_back( pState->second );

            // Finally notify the new state to initialze
            pState->second->OnEnter();

            mStateChanged = true;
        }
    }

    VOID GameDirector::PopState( VOID )
    {
        if( mStack.size() > 0 )
        {
            // First, notify the current state to clean up
            mStack.back()->OnExit();

            // Next remove the  current state from the stack
            mStack.pop_back(); 

            // Finally, notify the new top state to resume
            if( mStack.size() > 0 ) mStack.back()->OnResume();

            mStateChanged = true;
        }
    }

    VOID GameDirector::HandleInput( VOID )
    {
        if( mStateChanged == true && mInFrame ) return;

        GameState * pState = mStack.back();
        pState->HandleInput();
    }

    VOID GameDirector::Update( FLOAT elapsedTime )
    {
        if( mStateChanged == true && mInFrame ) return;

        GameState * pState = mStack.back();
        pState->Update(elapsedTime );
    }

    VOID GameDirector::Draw( VOID )
    {
        if( mStateChanged == true && mInFrame ) return;

        GameState * pState = mStack.back();
        pState->Draw();
    }
}