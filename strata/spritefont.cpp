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

#include <fstream>
#include <sstream>
#include <iostream>
#include "spritefont.h"



namespace Strata
{
    SpriteFont::SpriteFont( VOID )
    {
        mFontColor.AsDWORD = MAKE_RGBA( 255, 255, 255, 255 );
        mFontScale = 1.0f;
        mFontTexture = 0UL;
        mFontBlend = 1;
        mUseKern = TRUE;

    }
    SpriteFont::~SpriteFont( VOID )
    {

    }

    FLOAT SpriteFont::GetStringWidth( const char * str )
    {
        if( str == nullptr ) return 0.0f;

        float totalWidth = 0.0f;
        for( size_t idx = 0; idx != strlen(str); idx++ )
        {
           const CharDescriptor& f = mCharMap[str[idx]];
           totalWidth += f.XAdvance;
        }
        
        // Return total width after scaling
        return totalWidth * mFontScale;
    }

    int SpriteFont::GetKerningPair( int first, int second )
    {
        if( mKernCount > 0 && mUseKern == true )
        {
            for( int j = 0; j < mKernCount; j++ )
            {
                if( mKernList[j].First == first && mKernList[j].Second == second )
                {
                    return mKernList[j].Amount;
                }
            }
        }

        return 0;
    }

    bool SpriteFont::LoadFont( const std::string& fontName, const std::string& fontData, const std::string& fontTexture)
    {
        std::ifstream stream(fontData);
        if( !stream.is_open()) {
            return false;
        }
        stream.close();

        // Load font texture
        if( fontTexture.empty() == false  ) {
            mFontTexture = ResourceManager::AddTexture( fontName, 512.0f, 512.0f, fontTexture );
        }

        // Parse font data
        ParseFont( fontData );

        // Update kern count variable
        mKernCount = mKernList.size();

        // Return successfully
        return true;
    }

    bool SpriteFont::ParseFont( const std::string& fontFile )
    {
        std::ifstream stream(fontFile);
        std::string line;
        std::string read, key, value;
        std::size_t i;
        int first, second, amount;

        KerningInfo k;
        CharDescriptor c;

        while( !stream.eof() )
        {
            std::stringstream lineStream;
            std::getline( stream, line );
            lineStream << line;

            // Read this line's type
            lineStream >> read;
            if( read == "common" )
            {
                while( !lineStream.eof() )
                {
                    // Parse the common data
                    std::stringstream converter;
                    lineStream >> read;
                    i = read.find("=");
                    key = read.substr( 0, i );
                    value = read.substr( i + 1 );

                    // Assign the correct value
                    converter << value;
                    if( key == "lineHeight" ) { converter >> mLineHeight; }
                    else if( key == "base" ) { converter >> mBase; }
                    else if( key == "scaleW" ) { converter >> mWidth; }
                    else if( key == "scaleH" ) { converter >> mHeight; }
                    else if( key == "pages" ) { converter >> mPages; }
                }
            }
            else if( read == "char" )
            {
                // this is data for each character
                int charId = 0;

                // parse the char data
                while( !lineStream.eof() )
                {
                    std::stringstream converter;
                    lineStream >> read;
                    i = read.find("=" );
                    key = read.substr(0, i );
                    value = read.substr( i + 1);

                    // Assign the correct value
                    converter << value;
                    if( key == "id" ) { converter >> charId; }
                    else if( key == "x" ) { converter >> c.x; }
                    else if( key == "y") { converter >> c.y; }
                    else if( key == "width" ) { converter >> c.Width; }
                    else if( key == "height" ) { converter >> c.Height; }
                    else if( key == "xoffset" ) { converter >> c.XOffset; }
                    else if( key == "yoffset" ) { converter >> c.YOffset; }
                    else if( key == "xadvance" ) { converter >> c.XAdvance; }
                    else if( key == "page" ) { converter >> c.Page; }
                }

                // Add char to our map
                mCharMap.insert( std::make_pair( charId, c ));
            }
            else if( read == "kernings" )
            {
                // parse the kernings data
                while( !lineStream.eof() )
                {
                    std::stringstream converter;
                    lineStream >> read;
                    i = read.find("=");
                    key = read.substr( 0, i );
                    value = read.substr( i + 1);

                    // Assign the correct value
                    converter << value;
                    if( key == "count") { converter >> mKernCount; }
                }
            }
            else if( read == "kerning" )
            {
                // Parse kerning data
                while (!lineStream.eof() )
                {
                    std::stringstream converter;
                    lineStream >> read;
                    i = read.find("=");
                    key = read.substr( 0, i );
                    value = read.substr( i + 1) ;

                    // Assign the correct value
                    converter << value;
                    if( key == "first" ) { converter >> k.First; converter >> first; }
                    if( key == "second" ) { converter >> k.Second; converter >> second; }
                    if( key == "amount" ) { converter >> k.Amount; converter >> amount; }
                }

                // Add this kern info to our kern list
                mKernList.push_back( k );
            }
        }

        // Close file and return successfully
        stream.close();
        return true;
    }

    VOID SpriteFont::DrawText( Renderer& renderer, FLOAT x, FLOAT y, DWORD flags, DWORD layerId, const char * format, ... )
    {
        float curX = x;
        float curY = y;
        float dstX = 0.0f;
        float dstY = 0.0f;
        int fLen = 0;

        char text[512] = "";
        CharDescriptor * f = nullptr;

        va_list ap;
        if( format == nullptr ) return;

        va_start( ap, format );
        vsprintf( text, format, ap );
        va_end( ap );

        fLen = strlen( text );

        if( (flags & SPRITEFONT_ALIGN_RIGHT) == SPRITEFONT_ALIGN_RIGHT ) {
            x -= GetStringWidth(text);
        } else if( (flags & SPRITEFONT_ALIGN_CENTER) == SPRITEFONT_ALIGN_CENTER ) {
            x -= GetStringWidth(text)/2;
        }

        for( int i = 0; i < fLen; i++ )
        {
            f = &mCharMap[text[i]];
            curX = x + f->XOffset * mFontScale;
            curY = y - mBase * mFontScale + f->YOffset * mFontScale;
            dstX = f->Width * mFontScale;
            dstY = f->Height * mFontScale;

            const RECT destRect = { curX, curY, curX + dstX, curY + dstY };
            const RECT uvRect = { (FLOAT)f->x, (FLOAT)f->y, (FLOAT)f->x + (FLOAT)f->Width, (FLOAT)f->y + (FLOAT)f->Height };
            //std::cout << "Char:  " << text[i] << " l: " << destRect.left << " t: " << destRect.top << " r: " << destRect.right << " b: " << destRect.bottom << std::endl; 
            renderer.Draw( destRect, uvRect, mFontTexture, layerId, mFontColor );
            if( fLen > 1 && i < fLen ) {
                x += GetKerningPair( text[i], text[i+1] ) * mFontScale;
            }
            x+= f->XAdvance * mFontScale;
        }

    }

}