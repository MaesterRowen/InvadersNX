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

#include <switch.h>
#include <iostream>
#include "game/game.h"

//#define USE_NXLINK  1           // Define to enable use of nxlink

int main(int argc, char** argv) 
{
    #ifdef USE_NXLINK
        socketInitializeDefault();
        nxlinkStdio();
        std::cout << "nxlink initialized." << std::endl;
    #endif

    // Initialize romfs
    romfsInit();

    // Create an instance of our game class and run
    {
        Game gameInstance;
        gameInstance.Run();
    }

    // Cleanup romfs
    romfsExit();

    // Clean up nxlink, if enabled
    #ifdef USE_NXLINK
        socketExit();
    #endif

    // Terminate successfully
    return 0;
}
