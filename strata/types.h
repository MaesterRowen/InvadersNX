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

#ifndef _STRATA_TYPE_H_
#define _STRATA_TYPE_H_

// Definitions
#define FALSE 0
#define TRUE 1

// Constants
#define STRATA_PI  3.141593f

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long long QWORD;
typedef short SHORT;
typedef long LONG;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef float FLOAT;
typedef void VOID;
typedef unsigned int BOOL;

typedef struct _RECT 
{
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;
} RECT, *pRECT;

typedef union _LARGE_INTEGER {
    struct {
        DWORD HighPart;
        DWORD LowPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

#include "vector.h"
#include "color.h"

#endif