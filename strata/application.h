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

#ifndef _STRATA_APPLICATION_H_
#define _STRATA_APPLICATION_H_

#include <SDL2/SDL.h>
#include "types.h"
#include "timeinfo.h"

namespace Strata
{
    extern SDL_Renderer* gRenderer;


    class Application
    {
    public:

    protected:

        // SDL Variables
        SDL_Window* mpWindow;
        SDL_Renderer * mpRenderer;

        bool mRunning;

        virtual ~Application() {}

        // // Constructor
        Application() {
            mRunning = FALSE;
        }

    public:
        // Overridable Functions
        virtual BOOL Initialize() = 0;
        virtual DWORD FixedUpdate( FLOAT elapsedTime ) { return 0; }
        virtual DWORD Update(FLOAT elapsedTime ) = 0;
        virtual DWORD LateUpdate( FLOAT elapsedTime ) { return 0L; }
        virtual DWORD Render() = 0;

        // Function to create and run the application
        VOID Run();

        // Helper Methods
        SDL_Window* GetSDLWindow( VOID ) { return mpWindow; }
        SDL_Renderer* GetSDLRenderer( VOID ) { return mpRenderer; }

        // Access to the time class
        TimeInfo Time;
    };
}

#endif