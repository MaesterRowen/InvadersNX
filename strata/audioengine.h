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

#ifndef _STRATA_AUDIOENGINE_H_
#define _STRATA_AUDIOENGINE_H_

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <functional>

#include "types.h"

#define AUDIO_DEFAULT_VOLUME    64

namespace Strata
{   
    class AudioEngine 
    {
    public:
        AudioEngine( VOID );
        virtual ~AudioEngine( VOID );

        BOOL Initialize( int numMixingChannels =8 );
        VOID Destroy( VOID );

        VOID PauseAudio( VOID );
        VOID UnpauseAudio( VOID );
   
        BOOL LoadSound( const std::string& name, const std::string& filepath );
        BOOL LoadMusic( const std::string& name, const std::string& filepath );

        BOOL PlayMusic( const std::string& name, int volume = AUDIO_DEFAULT_VOLUME, int loops = -1 );
        BOOL PlaySound( const std::string& name, int volume = AUDIO_DEFAULT_VOLUME, int loops =  0 );

        BOOL IsPlaying( VOID );

    private:

        typedef struct _STRATA_MUSIC
        {
            Mix_Music * MusicData;
            BOOL Loaded;
            BOOL Playing;
            std::string Name;
            std::string FilePath;
            int Volume;
            int Loops;
        } STRATA_MUSIC, *pSTRATA_MUSIC;

        typedef struct _STRATA_SOUND 
        {
            Mix_Chunk * SoundData;
            BOOL Loaded;
            BOOL Playing;
            std::string Name;
            std::string FilePath;
            int Volume;
            int Loops;
        } STRATA_SOUND, *pSTRATA_SOUND;    

        std::unordered_map< std::string, STRATA_MUSIC*> mMusicMap;
        std::unordered_map< std::string, STRATA_SOUND*> mSoundMap;

        BOOL mAudioEnabled;
    };
}

#endif