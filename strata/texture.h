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

#ifndef _STRATA_TEXTURE_H_
#define _STRATA_TEXTURE_H_

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include "types.h"

namespace Strata 
{
	typedef unsigned long HTEXTURE;
	typedef unsigned short HREGION;

	class Texture 
	{
	public:
		Texture( const std::string& textureName, const std::string& texturePath, FLOAT width, FLOAT height, SDL_Texture * pTexture )
		{
			mTextureName = textureName;
			mTexturePath = texturePath;
			mpTexture = pTexture;
			mRegionHandleCounter = 0UL;


			// Fill in base texture information
			if( pTexture != nullptr )
			{
				mTextureExtents.left = 0UL;
				mTextureExtents.top = 0UL;
				mTextureExtents.right = (DWORD)width;
				mTextureExtents.bottom = (DWORD)height;
				mWidth = width;
				mHeight = height;
			}
		}

		const std::string& GetTextureName( VOID ) const { return mTextureName; }

		VOID Release( VOID )
		{
			if( mpTexture != nullptr ) {
				SDL_DestroyTexture( mpTexture );
				mpTexture = nullptr;
			}
		}

		HREGION AddRegion( const std::string& regionName, DWORD left, DWORD top, DWORD right, DWORD bottom )
		{
			HREGION hRegion = ++mRegionHandleCounter;
			RECT r = { (FLOAT)left, (FLOAT)top, (FLOAT)right, (FLOAT)bottom };
			mRegionMap.insert( std::make_pair( hRegion, r ) );
			mHandleMap.insert( std::make_pair( regionName, hRegion ) );
			return hRegion;
		}

		const RECT * GetRegion( HREGION hRegion ) const 
		{
			auto mit = mRegionMap.find( hRegion );
			if( mit == mRegionMap.end()) return &mTextureExtents;
			return &mit->second;
		}
		HREGION GetRegionHandle( const std::string& regionName )
		{
			auto mit = mHandleMap.find(regionName);
			if( mit == mHandleMap.end()) return 0UL;
			return mit->second;
		}
		SDL_Texture* GetTexture( VOID ) { return mpTexture; }
		FLOAT GetWidth( VOID ) const { return mWidth; }
		FLOAT GetHeight( VOID ) const { return mHeight; }

	protected:
		// Texture information
		RECT mTextureExtents;
		SDL_Texture* mpTexture;
		std::string mTexturePath;
		std::string mTextureName;
		FLOAT mWidth;
		FLOAT mHeight;

		// Map to hold region information
		std::unordered_map< HREGION, RECT > mRegionMap;
		std::unordered_map< std::string, HREGION > mHandleMap;
		WORD mRegionHandleCounter;
	};
}

#endif