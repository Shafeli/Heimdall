#pragma once

#include <string>
#include <unordered_map>
#include "../../AssetManager/AssetManager.h"
#include "Audio/BrokkrMusic.h"
#include "Audio/BrokkrSound.h"

/*
// https://lazyfoo.net/SDL_tutorials/lesson11/index.php

    //The music
    Mix_Music* music = NULL;

    //The sound effect that will be used
    Mix_Chunk* scratch = NULL;

    struct _Mix_Music is the data type we use for music Mix_Music is a typedef, and Mix_Chunk is the data type used for sound effects.
*/

namespace Brokkr
{

    class AudioManager final : public AssetSubsystem
    {
        std::unordered_map<std::string, std::unique_ptr<BrokkrMusic>> m_pMusic;
        std::unordered_map<std::string, std::unique_ptr<BrokkrSound>> m_pSounds;

    public:
        explicit AudioManager(AssetManager* assetManager);

        bool InitAudio(bool isChannelMono = false);
        void CloseAudio();

        void LoadMusic(const char* musicFilePath);
        void LoadSoundEffect(const char* soundEffectFilePath);

        bool PlaySoundEffect(const std::string& effectName, int loops = 0);
        bool PlayMusic(const std::string& musicName, int loops = 0);

        virtual void Destroy() override;

    };

}

