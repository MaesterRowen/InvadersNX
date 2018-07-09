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

#include "resourcemanager.h"
#include "texturecache.h"

namespace Strata 
{
    // Global Resource Cache
    TextureCache gTextureCache;

    SDL_Texture * ResourceManager::GetTexture( DWORD textureId )
    {
        return gTextureCache.GetTexture((HTEXTURE)textureId );
    }
    DWORD ResourceManager::AddTexture( const std::string& textureName, FLOAT width, FLOAT height, const std::string& filePath, const std::string& atlasPath /*= ""*/ )
    {
        return gTextureCache.AddTexture( textureName, width, height, filePath, atlasPath );
    }
    const RECT * ResourceManager::GetTextureRect( DWORD textureId )
    {
        return gTextureCache.GetTextureRect( (HTEXTURE)textureId );
    }
    const Texture * ResourceManager::GetTextureObject( DWORD textureId )
    {
        return gTextureCache.GetTextureObject( (HTEXTURE)textureId );
    }
    HTEXTURE ResourceManager::GetTextureHandle( const std::string& textureName )
    {
        return gTextureCache.GetTextureHandle( textureName );
    }
    HTEXTURE ResourceManager::GetTextureHandle( HTEXTURE hTexture, const std::string& regionName )
    {
        return gTextureCache.GetTextureHandle( hTexture, regionName );
    }

    VOID ResourceManager::DestroyCache( VOID )
    {
        gTextureCache.ClearTextureCache();
    }
}