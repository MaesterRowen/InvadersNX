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

#ifndef _STRATA_TEXTURECACHE_H_
#define _STRATA_TEXTURECACHE_H_

#include <unordered_map>
#include <string>
#include "Texture.h"

namespace Strata
{
    class TextureCache
    {
    public:
        TextureCache( VOID );
        ~TextureCache( VOID );

        // Public Methods for managing texture cache
        DWORD GetTextureCount( VOID );
        VOID ClearTextureCache( VOID );

        // Public methods for loading and unloading textures
        HTEXTURE AddTexture( const std::string& textureName, FLOAT width, FLOAT height, const std::string& filePath, const std::string& atlasPath = "" );
        VOID RemoveTexture( const std::string& textureName );

        // Public methods for accessing textures and regions
        HTEXTURE GetTextureHandle( HTEXTURE hTexture, const std::string& regionName );
        HTEXTURE GetTextureHandle( const std::string& textureName );
        const RECT * GetTextureRect( HTEXTURE hTexture ) const;
        SDL_Texture* GetTexture( HTEXTURE hTexture );
        const Texture * GetTextureObject( HTEXTURE hTexture );

    private:
        // Private variables
        DWORD mHandleCounter;
        // CRIT SECtION?
        std::unordered_map< HTEXTURE, Texture * const> mTextureMap;
        std::unordered_map< std::string, HTEXTURE> mHandleMap;
    };
}

#endif