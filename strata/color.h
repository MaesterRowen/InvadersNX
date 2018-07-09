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

#ifndef _STRATA_COLOR_H_
#define _STRATA_COLOR_H_

#include "types.h"

namespace Strata
{
    typedef union _COLOR {
        struct {
            BYTE r;
            BYTE g;
            BYTE b;
            BYTE a;
        } Components;
        DWORD AsDWORD;
    } COLOR, *pCOLOR;

    // Helper Macros
    #define MAKE_RGBA(r, g, b, a )  (((a << 24)& 0xFF000000) | ((b << 16) & 0xFF0000) | ((g << 8) & 0xFF00) | (r & 0xFF))
    #define MAKE_RGBX(r, g, b)      MAKE_RGBA(0xFF,b,g,r)
}

#endif