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

#ifndef _STRATA_SPRITEFONT_H_
#define _STRATA_SPRITEFONT_H_

#include <string>
#include <map>
#include <vector>
#include "types.h"
#include "resourcemanager.h"
#include "renderer.h"

namespace Strata 
{
    #define SPRITEFONT_ALIGN_LEFT       0x0
    #define SPRITEFONT_ALIGN_CENTER     0x1
    #define SPRITEFONT_ALIGN_RIGHT      0x2
    #define SPRITEFONT_ALIGN_CENTER_X   0x4
    #define SPRITEFONT_ALIGN_CENTER_Y   0x8

    struct txdata {
        float x, y;
        float tx, ty;
        BYTE r, g, b, a;
        txdata( float x, float y, float tx, float ty, BYTE r, BYTE g, BYTE b, BYTE a )
        {
            this->x = x;
            this->y = y;
            this->tx = tx;
            this->ty = ty;
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };

    class KerningInfo
    {
    public:
        short First;
        short Second;
        short Amount;

        KerningInfo() : First(0), Second(0), Amount(0) { }
    };

    class CharDescriptor
    {
    public:
        short x, y;
        short Width;
        short Height;
        short XOffset;
        short YOffset;
        short XAdvance;
        short Page;

        CharDescriptor() {
            x = 0;
            y = 0;
            Width = 0;
            Height = 0;
            XOffset = 0;
            YOffset = 0;
            XAdvance = 0;
            Page = 0;
        }
    };


    class SpriteFont
    {
    public:
        SpriteFont( VOID );
        virtual ~SpriteFont( VOID );

        FLOAT GetStringWidth( const char * str );
        VOID Initialize( VOID );
        bool LoadFont( const std::string& fontName, const std::string& fontData, const std::string& fontTexture );
        bool ParseFont( const std::string& fontFile );
        VOID SetScale( FLOAT scale ) { mFontScale = scale; }
        VOID SetColor( const COLOR& color ) { mFontColor = color; }
        VOID SetColor( DWORD color ) { mFontColor.AsDWORD = color; }
        VOID SetColor( BYTE r, BYTE g, BYTE b, BYTE a ) { mFontColor.AsDWORD = MAKE_RGBA(r, g, b, a ); }
        VOID DrawText( Renderer& renderer, FLOAT x, FLOAT y, DWORD flags, DWORD layerId, const char * format, ... );
        int GetKerningPair( int first, int second );

    private:
        SHORT mLineHeight;
        SHORT mBase;
        SHORT mWidth;
        SHORT mHeight;
        SHORT mPages;
        SHORT mOutline;
        SHORT mKernCount;
        BOOL mUseKern;

        std::map< int, CharDescriptor > mCharMap;
        std::vector< KerningInfo > mKernList;
        std::vector< txdata > mTxList;

        COLOR mFontColor;
        FLOAT mFontScale;
        DWORD mFontBlend;
        HTEXTURE mFontTexture;
    };
}

#endif 