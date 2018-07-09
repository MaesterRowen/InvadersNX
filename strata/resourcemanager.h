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

#ifndef _STRATA_RESOURCEMANAGER_H_
#define _STRATA_RESOURCEMANAGER_H_

#include <string>
#include "texture.h"

namespace Strata
{
    class ResourceManager
    {
    private:
        ResourceManager() { }
        ~ResourceManager() { }

    public:
        static SDL_Texture * GetTexture( DWORD textureId );
        static DWORD AddTexture( const std::string& textureName, FLOAT width, FLOAT height, const std::string& filePath, const std::string& atlasPath = "" );
        static const RECT * GetTextureRect( DWORD textureId );
        static const Texture * GetTextureObject( DWORD textureId );
        static HTEXTURE GetTextureHandle( const std::string& textureName );
        static HTEXTURE GetTextureHandle( HTEXTURE hTexture, const std::string& regionName );

        static VOID DestroyCache( VOID );
    };
}

#endif