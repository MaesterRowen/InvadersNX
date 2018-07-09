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

#ifndef _STRATA_PARTICLEENGINE_H_
#define _STRATA_PARTICLEENGINE_H_

#include "types.h"
#include "spriterenderer.h"
#include "particlebatch.h"

namespace Strata
{
    class ParticleEngine
    {
    public:
        ParticleEngine( VOID );
        virtual ~ParticleEngine( VOID );

        VOID AddParticles( ParticleBatch * particleBatch );
        VOID Update( FLOAT elapsedTime );
        VOID Draw( SpriteRenderer& renderer );
        VOID Flush( VOID );

        DWORD GetParticleCount( VOID ) { return mParticleCount; }

    private:
        std::vector< ParticleBatch*> mBatches;
        DWORD mParticleCount;
    };
}

#endif