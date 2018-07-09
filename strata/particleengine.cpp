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

#include "particleengine.h"

namespace Strata 
{

    ParticleEngine::ParticleEngine( VOID )
    {
        mParticleCount = 0UL;
    }
    ParticleEngine::~ParticleEngine( VOID )
    {
        Flush();
    }
    VOID ParticleEngine::AddParticles( ParticleBatch * particleBatch )
    {
        mBatches.push_back(particleBatch);
    }
    VOID ParticleEngine::Update( FLOAT elapsedTime )
    {
        for( DWORD i = 0; i < mBatches.size(); i++ )
        {
            mBatches[i]->Update( elapsedTime );
        }
    }
    VOID ParticleEngine::Draw( SpriteRenderer& renderer )
    {
        mParticleCount = 0UL;
        for( DWORD i = 0; i < mBatches.size(); i++ )
        {
            renderer.BeginDraw( Strata::SpriteRenderer::SORTTYPE::SORTTYPE_BACKTOFRONT );

            mParticleCount += mBatches[i]->Draw( renderer );

            renderer.EndDraw();
            renderer.Render();
        }   
    }
    VOID ParticleEngine::Flush( VOID )
    {
        for(DWORD i = 0; i < mBatches.size(); i++ )
        {
            delete mBatches[i];
            mBatches[i] = nullptr;
        }
        mBatches.clear();
    }

}