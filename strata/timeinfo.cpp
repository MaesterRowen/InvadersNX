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

#include <SDL2/SDL.h>
#include "timeinfo.h"

namespace Strata
{
    TimeInfo::TimeInfo( VOID )
    {
        // Initialize our variables
        mFrameCount.QuadPart = 0ULL;
        mSecondsPerTick = 0.0f;
        mApplicationTime = 0.0f;
        mElapsedTime = 0.0f;
        mRawApplicationTime.QuadPart = 0ULL;
        mRawTime.QuadPart = 0ULL;
        mTimeScale = 1.0f;
        mLastFPSTime = 0.0f;
        mFrameRate = 0.0f;
    }

    VOID TimeInfo::Initialize( VOID )
    {
        // Get the frequency of the timer
        LARGE_INTEGER qwTicksPerSec;
        qwTicksPerSec.QuadPart = SDL_GetPerformanceFrequency();
        mSecondsPerTick = (FLOAT)(1.0f / qwTicksPerSec.QuadPart );

        // Save the start time
        mRawTime.QuadPart = SDL_GetPerformanceCounter();

        // Zero out hte elapsed and total time variables
        mRawApplicationTime.QuadPart = 0ULL;
        mApplicationTime = 0.0f;
        mElapsedTime = 0.0f;
        mFrameCount.QuadPart = 0ULL;
    }

    VOID TimeInfo::EndFrame( VOID )
    {
        if( mApplicationTime - mLastFPSTime > 1.0f )
        {
            mFrameRate = mFrameCount.QuadPart / (mApplicationTime - mLastFPSTime);
            mFrameCount.QuadPart = 0;
            mLastFPSTime = mApplicationTime;
        }
    }

    VOID TimeInfo::UpdateTime( VOID )
    {
        LARGE_INTEGER qwNewTime;
        LARGE_INTEGER qwDeltaTime;

        qwNewTime.QuadPart = SDL_GetPerformanceCounter();
        qwDeltaTime.QuadPart = qwNewTime.QuadPart - mRawTime.QuadPart;

        mRawApplicationTime.QuadPart += qwDeltaTime.QuadPart;
        mRawTime.QuadPart = qwNewTime.QuadPart;

        mElapsedTime    = mSecondsPerTick * ((FLOAT)(qwDeltaTime.QuadPart));
        mApplicationTime = mSecondsPerTick * ((FLOAT)(mRawApplicationTime.QuadPart));

        mFrameCount.QuadPart++;
    }

}