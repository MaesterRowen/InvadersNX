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

#include "particlebatch.h"

#include "resourcemanager.h"

namespace Strata 
{
    ParticleBatch::ParticleBatch( VOID )
    {
        mDecayRate = 0.1f;
        mParticles = nullptr;
        mMaxParticles = 0UL;
        mLastFreeParticle = 0UL;
        mLayerId = 0UL;

    }
    ParticleBatch::~ParticleBatch( VOID )
    {
        if( mParticles ) delete [] mParticles;

    }
    VOID ParticleBatch::Initialize( DWORD maxParticles, FLOAT decayRate, DWORD layerId, HTEXTURE hTexture, std::function<void(Particle&, FLOAT)> updateFunc )
    {
        mMaxParticles = maxParticles;
        mDecayRate = decayRate;
        mTexture = hTexture;
        mLayerId = layerId;
        mUpdateFunc = updateFunc;

        // Determine texture
        const Texture * pTexture = ResourceManager::GetTextureObject( hTexture );
        if( pTexture ) {
            mTextureWidth = pTexture->GetWidth();
            mTextureHeight = pTexture->GetHeight();
        } else {
            // Use some default vaules
            mTextureWidth = 16.0f;
            mTextureHeight = 16.0f;
        }

        mParticles = new Particle[maxParticles];
    }
    VOID ParticleBatch::Update( FLOAT elapsedTime )
    {
        for( DWORD i = 0; i < mMaxParticles; i++ )
        {
            if( mParticles[i].Life > 0.0f ) {
                mUpdateFunc( mParticles[i], elapsedTime );
                mParticles[i].Life -= mDecayRate * elapsedTime;
            }
        }
    }
    DWORD ParticleBatch::Draw( Renderer& renderer )
    {
        const RECT uvRect = { 0.0f, 0.0f, mTextureWidth, mTextureHeight };
        DWORD counter = 0UL;
        for( DWORD i = 0; i < mMaxParticles; i++ )
        {
            // Check if the particle is active
            if( mParticles[i].Life > 0.0f )
            {
                const Particle& p = mParticles[i];
                const RECT destRect = { p.Position.x, p.Position.y, p.Position.x + p.Width, p.Position.y + p.Width };
                renderer.Draw( destRect, uvRect, mTexture, mLayerId, p.Color );
                counter++;
            }
        }

        return counter;
    }
    VOID ParticleBatch::AddParticle( const VECTOR3& position, const VECTOR3 velocity, FLOAT life, const COLOR& color, FLOAT width )
    {
        DWORD particleIndex = findFreeParticles();
        Particle& p = mParticles[particleIndex];
        p.Life = life;
        p.Position = { position.x - width / 2.0f, position.y - width / 2.0f, position.z };
        p.Velocity = velocity;
        p.Color = color;
        p.Width = width;          
    }
    int ParticleBatch::findFreeParticles()
    {
        for( DWORD i = mLastFreeParticle; i < mMaxParticles; i++ )
        {
            if( mParticles[i].Life <= 0.0f )
            {
                mLastFreeParticle = i;
                return i;
            }
        }
        for( DWORD i = 0; i < mLastFreeParticle; i++ )
        {
            if( mParticles[i].Life <= 0.0f ) 
            {
                mLastFreeParticle = i;
                return i;
            }
        }

        // No partciles are free so overwrite first
        return 0;
    }
}