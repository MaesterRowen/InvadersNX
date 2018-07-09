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

#include <algorithm>
#include <iostream>
#include "spriterenderer.h"
#include "resourcemanager.h"
#include "application.h"

namespace Strata 
{
    extern SDL_Renderer * gRenderer;

    SpriteRenderer::SpriteRenderer()
    {

    }
    
    SpriteRenderer::~SpriteRenderer()
    {

    }
    bool SpriteRenderer::Initialize( VOID )
    {
        return true;
    }
    VOID SpriteRenderer::Draw( const RECT& destRect, const RECT& uvRect, DWORD textureId, DWORD layerId, const COLOR& color, FLOAT angle /*= 0.0f*/)
    {
        // Don't render this sprite if the color factor is transparent
        if( color.Components.a == 0x00 ) return;

        //SpriteGlyph glyph(destRect, uvRect, textureId, layerId, color );
        mGlyphs.emplace_back( destRect, uvRect, textureId, layerId, color, angle );
    }
    VOID SpriteRenderer::BeginDraw( SORTTYPE sortType )
    {
        mSortType = sortType;
        mGlyphPointers.clear();
        mGlyphs.clear();
    }
    VOID SpriteRenderer::EndDraw( VOID )
    {
        // Set up all pointers for fast sorting
        mGlyphPointers.resize( mGlyphs.size() );
        for( DWORD idx = 0; idx < mGlyphs.size(); idx++ )
        {
            mGlyphPointers[idx] = &mGlyphs[idx];
        }

        // Sort our glyphs
        sortSpriteGlyphs();
    }
    VOID SpriteRenderer::Render( VOID )
    {
        if( gRenderer == nullptr ) return;
        
        SDL_Texture * pTexture = nullptr;
        DWORD lastTextureId = 0UL;

        for( DWORD idx = 0; idx < mGlyphPointers.size(); idx++ )
        {
            SpriteGlyph * glyph = mGlyphPointers[idx];
            if( glyph == nullptr ) continue;
            if( glyph->TextureId != lastTextureId ) {
                pTexture = ResourceManager::GetTexture( glyph->TextureId );
                lastTextureId = glyph->TextureId;
                if( pTexture == nullptr ) return;
            }
            SDL_SetTextureAlphaMod(pTexture, glyph->Color.Components.a );
            SDL_SetTextureColorMod(pTexture, glyph->Color.Components.r, glyph->Color.Components.g, glyph->Color.Components.b );

            if( glyph->Angle == 0.0f ) {
                SDL_RenderCopy( gRenderer, pTexture, &glyph->ClipRect, &glyph->DestRect );
            } else {
                SDL_RenderCopyEx( gRenderer, pTexture, &glyph->ClipRect, &glyph->DestRect, glyph->Angle, nullptr, SDL_FLIP_NONE );
            }
            
        }
    }

    VOID SpriteRenderer::sortSpriteGlyphs( VOID )
    {
        switch( mSortType )
        {
            case SORTTYPE_FRONTTOBACK:
                std::stable_sort( mGlyphPointers.begin(), mGlyphPointers.end(), compareFrontToBack );
                break;
            case SORTTYPE_BACKTOFRONT:
                std::stable_sort( mGlyphPointers.begin(), mGlyphPointers.end(), compareBackToFront );
                break;
            case SORTTYPE_TEXTURE:
            default:
                std::stable_sort( mGlyphPointers.begin(), mGlyphPointers.end(), compareTexture );
                break;
        }
    }

    bool SpriteRenderer::compareFrontToBack( const SpriteGlyph* left, const SpriteGlyph* right )
    {
        return left->LayerId < right->LayerId;
        
    }
    bool SpriteRenderer::compareBackToFront( const SpriteGlyph* left, const SpriteGlyph* right )
    {
        // If we are on the same layer then we need to sort back to front
        if( left->LayerId == right->LayerId ) {
            return left->TextureId < right->TextureId;
        }

        // Otehrwise the smaller layer numbers get rendered first
        return left->LayerId < right->LayerId;
    }
    bool SpriteRenderer::compareTexture( const SpriteGlyph* left, const SpriteGlyph* right )
    {
        return left->TextureId < right->TextureId;

    }
}