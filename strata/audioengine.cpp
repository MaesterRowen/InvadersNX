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
#include "audioengine.h"

// Definitions
#define AUDIO_FORMAT                AUDIO_S16LSB
#define AUDIO_FREQUENCY             48000
#define AUDIO_CHANNELS              2
#define AUDIO_SAMPLES               4096
#define AUDIO_MAX_SOUNDS            25

#define SDL_AUDIO_ALLOW_CHANGES     SDL_AUDIO_ALLOW_ANY_CHANGE

#define AUDIO_DEBUG                 1

namespace Strata 
{
    AudioEngine::AudioEngine( VOID )
    {
        // Initlaize some variables
        mAudioEnabled = FALSE;

        mMusicMap.clear();
        mSoundMap.clear();
    }
    AudioEngine::~AudioEngine( VOID )
    {
        // Destroy the audio engine
        Destroy();
    }

    BOOL AudioEngine::Initialize( int numMixingChannels /*=8*/ )
    {
        // If audio was not intialized, then fail to instanntiate AudioEngine
        if( SDL_WasInit( SDL_INIT_AUDIO ) == 0 )
        {
             std::cout << "SDL AUDIO is not initialized" << std::endl;
             mAudioEnabled = FALSE;
             return FALSE;
        }

        // Open our Audio device
        if( Mix_OpenAudio( AUDIO_FREQUENCY, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_SAMPLES ) != 0 )
        {
            std::cout << "Unable to open audio device.  Err:  " << Mix_GetError() << std::endl;
            mAudioEnabled = FALSE;
            return FALSE;
        }

        // Unset hooks, just in case
        Mix_HookMusic( NULL, NULL );
        Mix_HookMusicFinished( NULL );

        // Allocate additioanl mixing channels
        if( numMixingChannels != 8 ) 
            Mix_AllocateChannels(numMixingChannels);

        // Set the state to enabled
        mAudioEnabled = TRUE;

        // Unpause all audio streams
        UnpauseAudio();

        // Return successfully
        return TRUE;
    }
    VOID AudioEngine::Destroy( VOID )
    {
        // Disable our audio engine, so nothing else can play or load
        mAudioEnabled = FALSE;     

        // Loop through and destroy all of our music
        for( auto it = mMusicMap.begin(); it != mMusicMap.end(); ++it )
        {
            STRATA_MUSIC * music = it->second;
            if( music == nullptr ) continue;

            if( music->Playing == TRUE ) {
                Mix_HaltMusic();
                music->Playing = FALSE;
            }

            music->Loaded = FALSE;
            Mix_FreeMusic( music->MusicData );
            
            #ifdef AUDIO_DEBUG
              std::cout << "Unloaded Music:  " << music->Name << std::endl;
            #endif

            // Delete our music object
            delete music;
            music = NULL;            
        }

        // Loop through and destory all of our sounds
        for( auto it = mSoundMap.begin(); it != mSoundMap.end(); ++it )
        {
            STRATA_SOUND * sound = it->second;
            if( sound == nullptr ) continue;

            if( sound->Playing == TRUE ) {
                Mix_HaltChannel(-1);
                sound->Playing = FALSE;
            }

            sound->Loaded = FALSE;
            Mix_FreeChunk( sound->SoundData );

            #ifdef AUDIO_DEBUG
              std::cout << "Unloaded Sound:  " << sound->Name << std::endl;
            #endif

            // Delete our sound object
            delete sound;
            sound = NULL;
        }

        // Clear our maps
        mMusicMap.clear();
        mSoundMap.clear();

        // Finally, let's close our audio device
        Mix_CloseAudio();

        #ifdef AUDIO_DEBUG
            std::cout << "Audio Engine has been shutdown" << std::endl;
        #endif        
    }
    VOID AudioEngine::PauseAudio( VOID )
    {
        if( mAudioEnabled == TRUE )
        {
            if( Mix_Paused(-1) == FALSE ) Mix_Pause(-1);
            if( Mix_PausedMusic() == FALSE ) Mix_PauseMusic();
        }
    }
    VOID AudioEngine::UnpauseAudio( VOID )
    {
        if( mAudioEnabled == TRUE )
        {
            if( Mix_Paused(-1) == TRUE ) Mix_Resume(-1);
            if( Mix_PausedMusic() == TRUE ) Mix_ResumeMusic();
        }
    }
    BOOL AudioEngine::LoadSound( const std::string& name, const std::string& filepath )
    {
        if( mAudioEnabled == FALSE ) return FALSE;

        // Check if a sound with the requested name already exists, and if so, fail
        if( mSoundMap.count( name ) > 0 ) return FALSE;

        Mix_Chunk * soundData = Mix_LoadWAV( filepath.c_str() );
        if( soundData == nullptr ) return FALSE;

        STRATA_SOUND * sound = new STRATA_SOUND();
        if( sound == nullptr ) return FALSE;        

        // Fill out our sound struct       
        sound->FilePath = filepath;
        sound->Name = name;
        sound->Loops = 0;
        sound->Playing = FALSE;
        sound->Volume = 0;
        sound->Loaded = TRUE;
        sound->SoundData = soundData;

        #ifdef AUDIO_DEBUG
            std::cout << "Loaded Sound:  " << sound->Name << std::endl;
        #endif        

        // Add this sound to our sound map
        mSoundMap.insert( std::make_pair( name, sound ) );

        // Return successfully
        return TRUE;
    }
    BOOL AudioEngine::LoadMusic( const std::string& name, const std::string& filepath )
    {
        if( mAudioEnabled == FALSE ) return FALSE;

        // Check if music with the requested name already exists, and if so, fail
        if( mMusicMap.count( name ) > 0 ) return FALSE;        

        Mix_Music * musicData = Mix_LoadMUS( filepath.c_str() );
        if( musicData == nullptr ) return FALSE;

        STRATA_MUSIC * music = new STRATA_MUSIC();
        if( music == nullptr ) return FALSE;        

        // Fill out our sound struct       
        music->FilePath = filepath;
        music->Name = name;
        music->Loops = 0;
        music->Playing = FALSE;
        music->Volume = 0;
        music->Loaded = TRUE;
        music->MusicData = musicData;

        #ifdef AUDIO_DEBUG
            std::cout << "Loaded Music:  " << music->Name << std::endl;
        #endif            

        // Add this sound to our sound map
        mMusicMap.insert( std::make_pair( name, music ) );

        // Return successfully
        return TRUE;
    }
    BOOL AudioEngine::PlayMusic( const std::string& name, int volume /*= AUDIO_DEFAULT_VOLUME*/, int loops /* = -1*/ )
    {
        // If we are not enabled, then don't play
        if( mAudioEnabled == FALSE ) return FALSE;

        // Check if the name is in the music map
        if( mMusicMap.count( name ) == 0 ) return FALSE;

        // Let's play the music
        STRATA_MUSIC * music = mMusicMap[name];
        if( music == nullptr || music->Loaded == FALSE) return FALSE;

        // Set hte configuration
        music->Loops = loops;
        music->Playing = TRUE;
        music->Volume = volume;

        // Set hte volume
        Mix_VolumeMusic( volume );

        // Play the music
        Mix_PlayMusic( music->MusicData, loops );

        // Return successfully
        return TRUE;
    }
    BOOL AudioEngine::PlaySound( const std::string& name, int volume /*= AUDIO_DEFAULT_VOLUME*/, int loops /* = 0*/ )
    {
        // If we are not enabled, then don't play
        if( mAudioEnabled == FALSE ) return FALSE;

        // Check if the name is in the music map
        if( mSoundMap.count( name ) == 0 ) return FALSE;

        // Let's play the music
        STRATA_SOUND * sound = mSoundMap[name];
        if( sound == nullptr || sound->Loaded == FALSE) return FALSE;

        // Set hte configuration
        sound->Loops = loops;
        sound->Playing = TRUE;
        sound->Volume = volume;

        // Set hte volume
        Mix_Volume(-1, volume );

        // Play the music
        Mix_PlayChannel( -1, sound->SoundData, loops );

        // Return successfully
        return TRUE;
    }
    BOOL AudioEngine::IsPlaying( VOID )
    {
        // Return whether or not hte audio engine is playing anything
        return (mAudioEnabled == FALSE && (Mix_Playing(-1) == TRUE || Mix_PlayingMusic() == TRUE) ) ? TRUE : FALSE;
    }
}