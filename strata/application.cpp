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

#include <SDL2/SDL_image.h>
#include <switch.h>
#include <iostream>
#include "application.h"
#include "resourcemanager.h"

namespace Strata
{
    SDL_Renderer* gRenderer = nullptr;

    VOID Application::Run(void )
    {
        // Initialize SDL
        if( R_FAILED(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ))) {
            std::cout << "Failed to initialize SDL2.  Error:  " << SDL_GetError() << std::endl;
            return;
        }

        // Initialize IMG library
        IMG_Init(IMG_INIT_PNG);

        // Create a Window
        mpWindow = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_FULLSCREEN);
        if( mpWindow == nullptr ) {
            std::cout << "Failed to create SDL window.  Error:  " << SDL_GetError() << std::endl;
            // clean up SDL
            SDL_Quit();
            return;
        }

        // Create Renderer
        mpRenderer = SDL_CreateRenderer(mpWindow, 0, SDL_RENDERER_SOFTWARE );
        if( mpRenderer == nullptr ) {
            std::cout << "Failed to create SDL renderer.  Error:  " << SDL_GetError() << std::endl;
            // Clean up SDL
            SDL_DestroyWindow(mpWindow);
            SDL_Quit();
            return;
        }

        // Set the global renderer
        gRenderer = mpRenderer;

        // Initialize resources
        if( Initialize() == FALSE ) 
        {
            return;
        }

        // Initialize our timer
        Time.Initialize();

        // Initialization successful
        mRunning = true;
        while( appletMainLoop() && mRunning ) 
        {
            // Clear frame buffer
            SDL_SetRenderDrawColor( mpRenderer, 0x00, 0x00, 0x00, 0xff );
            SDL_RenderClear( mpRenderer);

            Time.UpdateTime();
            FLOAT elapsedTime = Time.GetElapsedTime();

            // Call fixed update (called at fixed intervals)
            FixedUpdate( elapsedTime );

            // Called once per frame
            Update( elapsedTime );

            // Call late update
            LateUpdate( elapsedTime );

            // Render
            Render();

            // Present
            SDL_RenderPresent(mpRenderer);

            // Tag the end of our frame
            Time.EndFrame();
        }

        // Destroy Texture Cache
        ResourceManager::DestroyCache();

        // Cleanup
        SDL_DestroyRenderer( mpRenderer );
        SDL_DestroyWindow( mpWindow );
        SDL_Quit();
        IMG_Quit();
    }
}