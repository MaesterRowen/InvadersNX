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

#ifndef _STRATA_SPRITEGLYPH_H_
#define _STRATA_SPRITEGLYPH_H_

#include <iostream>
#include <SDL2/SDL.h>
#include "types.h"

namespace Strata 
{
    class SpriteGlyph
    {
    public:
        //Constructor
        SpriteGlyph( const RECT& destRect, const RECT& uvRect, DWORD textureId, DWORD layerId, const COLOR& color, FLOAT angle = 0.0f )
        {
            DestRect.x = destRect.left;
            DestRect.y = destRect.top;
            DestRect.w = destRect.right - destRect.left;
            DestRect.h = destRect.bottom - destRect.top;

            ClipRect.x = uvRect.left;
            ClipRect.y = uvRect.top;
            ClipRect.w = uvRect.right - uvRect.left;
            ClipRect.h = uvRect.bottom - uvRect.top;

            // Set texture and layer ID
            Color = color;
            TextureId = textureId;
            LayerId = layerId;
            Angle = angle;
        }
        //SpriteGlyph( const VECTOR4& destRect, const VECTOR4& uvRect, DWORD textureId, DWORD layerId, const COLOR& color, FLOAT angle );
        
        // Copy Constructor
        SpriteGlyph( const SpriteGlyph& other )
        {
            DestRect = other.DestRect;
            ClipRect = other.ClipRect;

            TextureId = other.TextureId;
            LayerId = other.LayerId;
            Angle = other.Angle;
        }

        // Rects
        SDL_Rect DestRect;
        SDL_Rect ClipRect;

        // Rotation Angle
        FLOAT Angle; // degrees

        COLOR Color;

        // HAndle to a texture object
        DWORD TextureId;

        // Reference to the sprite layer
        DWORD LayerId;

    private:
        VECTOR3 rotatePoint( const VECTOR3& pos, FLOAT angle );
    };
}

#endif