// Invaders NX
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

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include <strata/strata.h>


class Explosion : public Strata::Sprite
{
public:
    Explosion( const Strata::VECTOR3& position, const Strata::VECTOR3& direction, float speed, float type, float timeToLive );
    virtual ~Explosion( VOID );
    BOOL Update( FLOAT elapsedTime );

private:
    FLOAT mTimeToLive;
    Strata::VECTOR3 mDirection;
    FLOAT mSpeed;
};

#endif