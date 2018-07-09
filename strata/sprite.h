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

#ifndef _STRATA_SPRITE_H_
#define _STRATA_SPRITE_H_

#include <string>
#include <unordered_map>
#include "types.h"
#include "renderer.h"
#include "texture.h"

namespace Strata 
{
	// Preprocessor definitions
	#define SPRITEANIM_LOOP_NONE		0
	#define SPRITEANIM_LOOP_ONCE		1
	#define SPRITEANIM_LOOP_INFINITE	0xffffffff

    class Sprite 
    {
    public:
        typedef struct _ANIMATION {
            std::string Name;
            DWORD InitialFrame;
            DWORD StartFrame;
            DWORD LastFrame;
            FLOAT Duration;
            FLOAT TimePerFrame;
        } ANIMATION, *pANIMATION;

        Sprite( VOID );
        virtual ~Sprite( VOID );

        VOID Initialize( FLOAT xPosition, FLOAT yPosition, FLOAT width, FLOAT height, DWORD layerId, HTEXTURE hTexture );
        VOID ConfigureAnimation( FLOAT frameWidth, FLOAT frameHeight, DWORD framesPerRow );
        virtual DWORD UpdateAnimation( FLOAT elapsedTime ) { return updateAnimation( elapsedTime ); }
        BOOL Draw( Renderer& renderer );

		// ************************************************************************************************
		//  DefineAnimation - Define sprite animation parameters
		//  -----------------------------------------------------------------------------------------------
		//  animationName : name of the animation to be referenced when playing
		//  initialFrame : the frame that starts playing when an animation is started
		//  starFrame : the frame that is looped back to when playing a loopable animation
		//  lastFrame : the last frame of the animation
		//  animDuration : length of time (in seconds) that an animation takes to complete one sequence
		// ************************************************************************************************
        BOOL DefineAnimation( const std::string& animationName, DWORD initialFrame, DWORD startFrame, DWORD lastFrame, FLOAT animDuration );
        BOOL DefineAnimation( const std::string& animationNAme, DWORD initialFrame, DWORD lastFrame, FLOAT animDuration );

        BOOL PlayAnimation( const std::string& animationName, DWORD numLoops = SPRITEANIM_LOOP_NONE );
        BOOL StopAnimation( VOID )      { mAnimationName = ""; return TRUE; }
        VOID PauseAnimation( VOID )     { mAnimationPaused = true; }
        VOID ResumeAnimation( VOID )    { mAnimationPaused = false; }

        // Getters
        const LARGE_INTEGER& GetSortKey( VOID ) { return mSortKey; }
        bool GetIsEnabled( VOID )               { return mEnabled; }
        bool GetIsVisible( VOID )               { return mVisible; }
        bool GetIsAnimated( VOID )              { return mAnimated; }
        bool GetIsAnimationPaused( VOID )       { return mAnimationPaused; }
        const VECTOR3& GetPosition( VOID )      { return mPosition; }
        const VECTOR3& GetScale( VOID )         { return mScale; }
        FLOAT GetWidth( VOID )                  { return mSpriteWidth; }
        FLOAT GetHeight( VOID )                 { return mSpriteHeight; }
        const COLOR& GetColorFactor( VOID )     { return mColor; }
        DWORD GetLayerId( VOID )                { return mLayerId; }
        HTEXTURE GetTexture( VOID )             { return mTextureHandle; }
        DWORD GetAnimationLoopCount( VOID )     { return mNumberOfLoops; }
        FLOAT GetAnimationSpeed( VOID )         { return mAnimationSpeed; }
        FLOAT GetX( VOID )                      { return mPosition.x; }
        FLOAT GetY( VOID )                      { return mPosition.y; }

        // Setters
        VOID SetEnabled( bool value )           { mEnabled = value; }
        VOID SetVisible( bool value )           { mVisible = value; }
        VOID SetAnimationLoops( DWORD loops )   { mNumberOfLoops = loops; }
        VOID SetAnimationSpeed( FLOAT speed )   { mAnimationSpeed = speed; }
        VOID SetColorFactor( const COLOR& col ) { mColor = col; }

        VOID SetColorFactor( BYTE r, BYTE g, BYTE b ) { mColor.AsDWORD = MAKE_RGBA(r, g, b, mColor.Components.a); }
        VOID SetAlphaFactor( BYTE a )           { mColor.Components.a = a; }

		// Geometric Transformations
		VOID Translate( FLOAT x, FLOAT y )		{ mPosition.x = x; mPosition.y = y; }
		VOID Scale( FLOAT x, FLOAT y )			{ mScale.x = x; mScale.y = y; }
		VOID Rotate( FLOAT angle )				{ mRotationAngle = angle; }        
        VOID SetWidth( FLOAT width )			{ mSpriteWidth = width; }

    protected:
        // Notification callbacks for derived class
        virtual VOID OnAnimationStart( const std::string& animationName ) { }
        virtual VOID OnAnimationEnd( const std::string& animationName ) { }

    protected:
        // General information variables
        bool mEnabled;                      // Flag to determine if this sprite is enabled (ie, does it update & render)
        bool mVisible;                      // Flag to determine if this sprite is visible (ie, does it render)
        bool mAnimated;                     // Flag to determine if this sprite is animated
        bool mAnimationPaused;              // Flag to determine if this sprite animation is paused

        // Rendering information variables
        FLOAT mSpriteWidth;
        FLOAT mSpriteHeight;
        VECTOR3 mPosition;
        VECTOR3 mScale;
        FLOAT mRotationAngle;
        DWORD mLayerId;
        COLOR mColor;
        LARGE_INTEGER mSortKey;

        // Animation information variables
        std::string mAnimationName;
        DWORD mCurrentLoop;
        DWORD mNumberOfLoops;
        DWORD mCurrentFrame;
        FLOAT mAnimationTimer;
        FLOAT mLastUpdateTime;
        FLOAT mAnimationSpeed;

        // Texture information variables
        HTEXTURE mTextureHandle;
        FLOAT mFrameWidth;
        FLOAT mFrameHeight;
        DWORD mFrameCount;
        DWORD mFramesPerRow;

        // Map of animations searchable by name
        std::unordered_map< std::string, ANIMATION > mAnimationMap;

    private:
        // Internal Methods
        BOOL updateAnimation( FLOAT elapsedTime );
        VOID calculateSortKey( VOID );
    };
}

#endif 