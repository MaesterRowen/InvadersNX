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

#ifndef _STRATA_SPRITERENDERER_H_
#define _STRATA_SPRITERENDERER_H_

#include <vector>
#include "spriteglyph.h"
#include "renderer.h"
#include "types.h"

namespace Strata
{
    class SpriteRenderer : public Renderer
    {
    public:
        typedef enum _SORTTYPE {
            SORTTYPE_FRONTTOBACK,
            SORTTYPE_BACKTOFRONT,
            SORTTYPE_TEXTURE
        } SORTTYPE;

        SpriteRenderer();
        virtual ~SpriteRenderer();
    

        bool Initialize( VOID );
        VOID Release( VOID ) {}
        VOID Draw( const RECT& destRect, const RECT& uvRect, DWORD textureId, DWORD layerId, const COLOR& color, FLOAT angle = 0.0f ) override;
        VOID Render( VOID );

        VOID BeginDraw( SORTTYPE sortType = SORTTYPE::SORTTYPE_BACKTOFRONT );
        VOID EndDraw( VOID );

    private:
        std::vector< SpriteGlyph >  mGlyphs;
        std::vector< SpriteGlyph* > mGlyphPointers;

        SORTTYPE mSortType;
        VOID sortSpriteGlyphs( VOID );

        static bool compareFrontToBack( const SpriteGlyph* left, const SpriteGlyph* right );
        static bool compareBackToFront( const SpriteGlyph* left, const SpriteGlyph* right );
        static bool compareTexture( const SpriteGlyph* left, const SpriteGlyph* right );
    };
}

#endif