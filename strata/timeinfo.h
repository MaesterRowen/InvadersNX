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

#ifndef _STRATA_TIMEINFO_H_
#define _STRATA_TIMEINFO_H_

#include "types.h"
namespace Strata
{
    class TimeInfo
    {
    private:
        // Private variables
        FLOAT mApplicationTime;
        FLOAT mElapsedTime;
        FLOAT mTimeScale;
        FLOAT mSecondsPerTick;
        FLOAT mLastFPSTime;
        LARGE_INTEGER mRawTime;
        LARGE_INTEGER mRawApplicationTime;
        LARGE_INTEGER mFrameCount;
        FLOAT mFrameRate;

    public:
        // Constructor
        TimeInfo(void);

        // Functional methods
        VOID Initialize( VOID );
        VOID UpdateTime( VOID );

        // Accessor methods
        FLOAT GetFrameRate()                    { return mFrameRate; }
        VOID EndFrame();
        FLOAT GetApplicationTime()              { return mApplicationTime; }
        FLOAT GetElapsedTime()                  { return mElapsedTime; }
        FLOAT GetTimeScale()                    { return mTimeScale; }
        VOID SetTimeScale( FLOAT timeScale )    { mTimeScale = timeScale; }
    };

}

#endif