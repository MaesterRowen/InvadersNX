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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "application.h"
#include "texturecache.h"
    
namespace Strata
{
    extern SDL_Renderer* gRenderer;

    TextureCache::TextureCache( VOID ) :
        mHandleCounter(0)
    {
        // Clear our data that may be in maps
        mTextureMap.clear();
        mHandleMap.clear();
    }
    TextureCache::~TextureCache( VOID )
    {
        // Free our cache
        ClearTextureCache();
    }
    
    // Public Methods for managing texture cache
    DWORD TextureCache::GetTextureCount( VOID )
    {
        return mTextureMap.size();
    }
    VOID TextureCache::ClearTextureCache( VOID ) 
    { 
        // Loop through each texture in teh cahse and destroy it
        for( auto it = mTextureMap.begin(); it != mTextureMap.end(); ++it )
        {
            Texture * item = it->second;
            if( item == nullptr ) continue;

            std::cout << "Release Texture:  '" << item->GetTextureName() << "'" << std::endl;

            // Release this texture
            item->Release();

            // Delete the pointer
            delete item;
        }

        // Clear our maps
        mTextureMap.clear();
        mHandleMap.clear();

        // Textures freed
        
    }

    // Public methods for loading and unloading textures
    HTEXTURE TextureCache::AddTexture( const std::string& textureName, FLOAT width, FLOAT height, const std::string& filePath, const std::string& atlasPath /*= ""*/ )
    {
        auto mit = mHandleMap.find( textureName );
        if( mit == mHandleMap.end() )
        {
            HTEXTURE hTexture = (HTEXTURE)0UL;
            SDL_Texture * newTexture = nullptr;

            // Open file from disc
            SDL_Surface * pSurface = IMG_Load(filePath.c_str());
            if( pSurface ) {
                newTexture = SDL_CreateTextureFromSurface( gRenderer, pSurface );
                SDL_FreeSurface(pSurface); 
                pSurface = nullptr;
            } else {
                std::cout << "Error loading texture: " << IMG_GetError() << std::endl;
            }

            if( newTexture != nullptr )
            {
                hTexture = ((++mHandleCounter << 16) & 0xFFFF0000 ) | 0xF0000000;
                Texture * pTexObj = new Texture(textureName, filePath, width, height, newTexture );
                if( pTexObj && atlasPath.empty() == false )
                {
                    // Loop through map file and create region objects
                    std::ifstream mapFile( atlasPath );
                    std::string mapEntry;
                    while( std::getline( mapFile, mapEntry ))
                    {
                        std::vector<std::string> parts;
                        for( std::string::size_type pos = mapEntry.find(","); mapEntry.length() > 0 && pos != mapEntry.npos; pos = mapEntry.find( "," ))
                        {
                            if( pos > 0 ) {
                                parts.push_back( mapEntry.substr( 0, pos ));
                            }
                            
                            mapEntry = mapEntry.substr( pos + 1);
                        }

                        if( mapEntry.length() > 0 ) parts.push_back(mapEntry);
                        if( parts.size() != 5 ) continue;

                        HREGION hRegion = pTexObj->AddRegion( parts[0],atoi( parts[1].c_str() ), atoi( parts[2].c_str() ), atoi( parts[3].c_str() ), atoi( parts[4].c_str() ) );
                        if( hRegion > 0 ) {
                            HTEXTURE fullTex = (hTexture & 0xFFFF0000) | hRegion;
                            std::string regionName = textureName + "." + parts[0];
                            mHandleMap.insert( std::make_pair( regionName, fullTex ));
                            std::cout << regionName << " : " << fullTex << std::endl;
                            std::cout << "[" << atoi( parts[1].c_str()) << ", " << atoi( parts[2].c_str()) << ", " << atoi( parts[3].c_str()) << ", " << atoi( parts[4].c_str()) << "]" << std::endl;
                        }
                    }
                }

                // Add our texture to the cache
                mHandleMap.insert( std::make_pair( textureName, hTexture ));
                mTextureMap.insert( std::make_pair( hTexture, pTexObj ));
            }
            else 
            {
                // Failed to load texture, so let's show our debug texture
                newTexture = nullptr;
            }

            return hTexture;
        }

        // Return handle
        return mit->second;
    }
    VOID TextureCache::RemoveTexture( const std::string& textureName )
    {

    }

    // Public methods for accessing textures and regions
    HTEXTURE TextureCache::GetTextureHandle( HTEXTURE hTexture, const std::string& regionName )
    {
        auto mit = mTextureMap.find(hTexture);
        if( mit == mTextureMap.end() ) return (HTEXTURE)0UL;
        return hTexture | (mit->second->GetRegionHandle( regionName ) & 0xFFFF );
    }
    HTEXTURE TextureCache::GetTextureHandle( const std::string& textureName )
    {
        auto mit = mHandleMap.find(textureName);
        if( mit == mHandleMap.end() ) return (HTEXTURE)0UL;
        return mit->second;
    }
    const RECT * TextureCache::GetTextureRect( HTEXTURE hTexture ) const
    {
        auto mit = mTextureMap.find( hTexture & 0xFFFF0000 );
        if( mit == mTextureMap.end() ) return nullptr;
        return mit->second->GetRegion( hTexture & 0xFFFF );
    }
    SDL_Texture* TextureCache::GetTexture( HTEXTURE hTexture )
    {
        auto mit = mTextureMap.find( hTexture & 0xFFFF0000 );
        if( mit == mTextureMap.end() ) return nullptr;

        return mit->second->GetTexture();
    }
    const Texture * TextureCache::GetTextureObject( HTEXTURE hTexture )
    {
        auto mit = mTextureMap.find( hTexture & 0xFFFF0000 );
        if( mit == mTextureMap.end() ) return nullptr;
        return mit->second;
    }
}