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

#ifndef _STRATA_PARTICLEBATCH_H_
#define _STRATA_PARTICLEBATCH_H_

#include <functional>
#include "texture.h"
#include "renderer.h"
#include "types.h"

namespace Strata
{
    class Particle
    {
    public:
        Particle( VOID )
        {
            Position.x = 0.0f; Position.y = 0.0f;
            Velocity.x = 0.0f; Velocity.y = 0.0f;
            Color.AsDWORD = MAKE_RGBA(255, 255, 255, 255);
            Life = 0.0f;
        }

        VECTOR3 Position;
        VECTOR3 Velocity;
        COLOR Color;
        FLOAT Width;
        FLOAT Life;
    };

    inline VOID DefaultParticleUpdate( Particle& particle, FLOAT elapsedTime )
    {
        particle.Position.x += particle.Velocity.x * elapsedTime;
        particle.Position.y += particle.Velocity.y * elapsedTime;
    }

    class ParticleBatch
    {
    public:
        ParticleBatch( VOID );
        virtual ~ParticleBatch( VOID );

        VOID Initialize( DWORD maxParticles, FLOAT decayRate, DWORD layerId, HTEXTURE hTexture, std::function<void(Particle&, FLOAT)> updateFunc = DefaultParticleUpdate );
        VOID Update( FLOAT elapsedTime );
        DWORD Draw( Renderer& renderer );

        VOID AddParticle( const VECTOR3& postion, const VECTOR3 velocity, FLOAT life, const COLOR& color, FLOAT width );
    private:
        int findFreeParticles();

        FLOAT mDecayRate;
        Particle * mParticles;
        DWORD mMaxParticles;
        HTEXTURE mTexture;
        DWORD mLastFreeParticle;
        DWORD mLayerId;

        FLOAT mTextureWidth;
        FLOAT mTextureHeight;

        std::function<void(Particle&, FLOAT)> mUpdateFunc;
    };

}

#endif