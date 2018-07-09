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

#ifndef _STRATA_RENDERER_H_
#define _STRATA_RENDERER_H_

#include <iostream>
#include <SDL2/SDL.h>
#include "types.h"
#include "application.h"

namespace Strata 
{
    class Renderer {

    public:
        virtual VOID Draw( const RECT& destRect, const RECT& uvRect, DWORD textureId, DWORD layerId, const COLOR& color, FLOAT angle = 0.0f ) {}
        virtual VOID DrawLine( FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, const COLOR& color )
        {
            SDL_SetRenderDrawColor( gRenderer, color.Components.r, color.Components.g, color.Components.b, color.Components.a );
            SDL_RenderDrawLine( gRenderer, (int) x1, (int)y1, (int)x2, (int)y2 );
        }
        virtual VOID DrawRect( FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, const COLOR& color, bool fill )
        {
            // Set the render color
            SDL_SetRenderDrawColor( gRenderer, color.Components.r, color.Components.g, color.Components.b, color.Components.a );

            SDL_Rect rect = { (int)x1, (int)y1, (int)(x2 - x1), (int)(y2 - y1) };

            if( fill == false ) {
                SDL_RenderDrawRect( gRenderer, &rect );
            } else {
                SDL_RenderFillRect( gRenderer, &rect );
            }
        }
    protected:

    protected:
        Renderer( ) { }
        virtual ~Renderer() { 
        }
        virtual bool Initialize() = 0;
        virtual VOID Render() = 0;
    };
}

#endif