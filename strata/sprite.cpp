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

#include <iostream>
#include "sprite.h"
#include "resourcemanager.h"
#include "texture.h"

namespace Strata 
{
    Sprite::Sprite( VOID )
    {
        // General Variables
        mEnabled = false;
        mVisible = false;
        mAnimated = false;
        mAnimationPaused = false;

        // Render variables
        mSpriteWidth = 64.0f;
        mSpriteHeight = 64.0f;
        mPosition.x = 0.0f;
        mPosition.y = 0.0f;
        mScale.x = 1.0f;
        mScale.y = 1.0f;
        mRotationAngle = 0.0f;
        mLayerId = 0UL;
        mColor.AsDWORD = 0xFFFFFFFF;
        mSortKey.QuadPart = 0ULL;

        // Animation variables
        mAnimationName = "";
        mCurrentLoop = 0UL;
        mNumberOfLoops = SPRITEANIM_LOOP_NONE;
        mCurrentFrame = 0UL;
        mAnimationTimer = 0.0f;
        mLastUpdateTime = 0.0f;
        mAnimationSpeed = 1.0f;

        // Texture Variables
        mTextureHandle = (HTEXTURE)0UL;
        mFrameWidth = 64.0f;
        mFrameHeight = 64.0f;
        mFrameCount = 1;
        mFramesPerRow = 1;
    }

    Sprite::~Sprite( VOID )
    {
        mAnimated = false;
        mAnimationPaused = false;
        mAnimationMap.clear(); 
    }

    VOID Sprite::Initialize( FLOAT xPosition, FLOAT yPosition, FLOAT width, FLOAT height, DWORD layerId, HTEXTURE hTexture )
    {
        // Setup sprite
        mPosition.x = xPosition;
        mPosition.y = yPosition;
        mScale.x = 1.0f;
        mScale.y = 1.0f;
        mSpriteWidth = width;
        mSpriteHeight = height;
        mLayerId = layerId;
        mTextureHandle = hTexture;
        mEnabled = true;
        mVisible = true;

        // Calculate sortkey
        calculateSortKey();

    }

    VOID Sprite::ConfigureAnimation( FLOAT frameWidth, FLOAT frameHeight, DWORD framesPerRow )
    {
        // Initialize some animation variables
        mFramesPerRow = framesPerRow;
        mFrameWidth = frameWidth;
        mFrameHeight = frameHeight;
    }

    BOOL Sprite::DefineAnimation( const std::string& animationName, DWORD initialFrame, DWORD lastFrame, FLOAT animDuration ) {
        return DefineAnimation( animationName, initialFrame, initialFrame, lastFrame, animDuration );
    }
    BOOL Sprite::DefineAnimation( const std::string& animationName, DWORD initialFrame, DWORD startFrame, DWORD lastFrame, FLOAT animDuration ) 
    {
        // Check if this animation already exists
        if( mAnimationMap.count(animationName) > 0 ) return FALSE;

        // Setup animation structure
        ANIMATION anim;
        anim.Name = animationName;
        anim.InitialFrame = initialFrame;
        anim.StartFrame = startFrame;
        anim.LastFrame = lastFrame;
        anim.Duration = animDuration;
        anim.TimePerFrame = animDuration / (lastFrame - initialFrame + 1);

        // Add this animation to our map
        mAnimationMap.insert( std::make_pair( animationName, anim ) );

        // Setup our internal state to denote that this sprite is animated
        mAnimated = true;

        // Return successfully
        return TRUE;
    }

    BOOL Sprite::Draw( Renderer& renderer )
    {
        // If this object is not enabled/visible, then it shouldn't be rendered
        if( mEnabled == false || mVisible == false ) return FALSE;

        // Retrieve a pointer to this sprites texture object
        const Texture * pTexture = ResourceManager::GetTextureObject( mTextureHandle );
        if( pTexture != nullptr )
        {
            // Retrieve a pointer to the proper texture region
            const RECT * pRect = pTexture->GetRegion( mTextureHandle & 0xFFFF );
            if( pRect != nullptr )
            {
                RECT uvRect = { 0 };

                // Cache the texture width and height
                //FLOAT texWidth = pTexture->GetWidth();
                //FLOAT texHeight = pTexture->GetHeight();

                // check if this is an animated sequence
                if( mAnimated == true )
                {
                    DWORD frameX = mCurrentFrame % mFramesPerRow;
                    DWORD frameY = mCurrentFrame / mFramesPerRow;

                    //std::cout << "FrameX: " << frameX << ", FrameY: " << frameY << std::endl;

                    // Create our clip rect
                    uvRect.left = pRect->left + frameX * mFrameWidth;
                    uvRect.top = pRect->top + frameY * mFrameHeight;
                    uvRect.right = uvRect.left + mFrameWidth;
                    uvRect.bottom = uvRect.top + mFrameHeight;

                    //std::cout << "l: " << uvRect.left << "r: " << uvRect.right << std::endl;
                }
                else
                {
                    uvRect.left = pRect->left;
                    uvRect.right = pRect->right;
                    uvRect.top = pRect->top;
                    uvRect.bottom = pRect->bottom;
                }

                // Create our position information
                RECT destRect = { mPosition.x, mPosition.y, (mSpriteWidth * mScale.x + mPosition.x), (mSpriteHeight * mScale.y + mPosition.y) };

                // Draw to sprite batch
                //if( mRotationAngle > 0.0f ) renderer->Draw( destRect, uvRect, mTextureHandle, mLayerId, mColor, mRotationAngle );
                /*else */

                renderer.Draw(destRect, uvRect, mTextureHandle, mLayerId, mColor, mRotationAngle );
            }
        }

        // Failed to properly render sprite
        return FALSE;
    }

    BOOL Sprite::PlayAnimation( const std::string& animationName, DWORD numLoops )
    {
        // Verify that the requested animation exists
        if( mAnimationMap.count( animationName ) == 0 ) return FALSE;

        // Reset our variables to allow the anmimation to change to the next
        mCurrentFrame = mAnimationMap[animationName].InitialFrame;
        mAnimationTimer = 0.0f;
        mLastUpdateTime = 0.0f;
        mNumberOfLoops = numLoops;
        mCurrentLoop = 0UL;
        mAnimationName = animationName;

        // Notify parent that a new animation has started 
        OnAnimationStart( animationName );

        // Return successfully
        return TRUE;
    }

    BOOL Sprite::updateAnimation( FLOAT elapsedTime )
    {
        //std::cout << "Time:  " << elapsedTime << std::endl;
        // If the sprite is not enabled, then there is nothing to do
        if( mEnabled == false ) return FALSE;

        // Check if this sprite is animated
        if( mAnimated == true )
        {
            // If the sprite is not paused, then increment our animation timer
            if( mAnimationPaused == false ) {
                mAnimationTimer += elapsedTime * mAnimationSpeed;
            }

            // Obtain a reference to the currently active animation
            auto anim = mAnimationMap.find(mAnimationName);
            if( anim == mAnimationMap.end() ) { return FALSE; }

            // Check to see if its time to update the current frame
            if( mAnimationTimer - mLastUpdateTime >= anim->second.TimePerFrame)
            {
                // Increment the frame counter
                mCurrentFrame++;
                //std::cout << "Current Frame:  " << mCurrentFrame << std::endl;

                // Check if the animation is at the last frame
                if( mCurrentFrame > anim->second.LastFrame )
                {
                    if( mCurrentLoop < mNumberOfLoops )
                    {
                        // Move the current frame back to the loop point
                        mCurrentFrame = anim->second.StartFrame;

                        // Increment the loop counter
                        mCurrentLoop++;
                        if( mNumberOfLoops == SPRITEANIM_LOOP_INFINITE ) {
                            mCurrentLoop = 0;
                        }
                    }
                    else 
                    {
                        // No more loops so the animation should stop
                        // at the last frame
                        mCurrentFrame = anim->second.LastFrame;
                    }
                }

                // Update the last check time
                mLastUpdateTime = mAnimationTimer;
            }
        }
        else 
        {
            // This sprite is not animated so set some values
            // that will cause the sprite to display the first frame
            // of the texture/region
            mAnimationTimer = 0.0f;
            mCurrentFrame = 0UL;
            mAnimationName = "";
            mCurrentLoop = 0UL;
            mLastUpdateTime = 0.0f;
        }

        // Return
        return TRUE;
    }

    VOID Sprite::calculateSortKey( VOID )
    {
        mSortKey.u.HighPart = mLayerId;
        mSortKey.u.LowPart = mTextureHandle;
        //std::cout << "LayerID:  " << mLayerId << ", Texture Handle:  " << mTextureHandle << "Sort Key:  " << (mSortKey.QuadPart & 0xFFFFFFFF) << "(" << mSortKey.u.HighPart << ", " << mSortKey.u.LowPart << ")" << std::endl;
    }
}