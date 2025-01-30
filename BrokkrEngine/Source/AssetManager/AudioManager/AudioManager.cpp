#include "AudioManager.h"

#include <SDL_mixer.h>

Brokkr::AudioManager::AudioManager(AssetManager* assetManager)
    :AssetSubsystem(assetManager)
{

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) 
    {
        // log things here
    }
}

bool Brokkr::AudioManager::InitAudio(bool isChannelMono)
{
    if (isChannelMono)
    {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096) == -1)
        {
            return false;
        }

        return true;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }

    return true;
}

void Brokkr::AudioManager::CloseAudio()
{
    Mix_CloseAudio();
}

void Brokkr::AudioManager::LoadMusic(const char* musicFilePath)
{
    m_pMusic.emplace(musicFilePath, std::make_unique<BrokkrMusic>(musicFilePath));
}

void Brokkr::AudioManager::LoadSoundEffect(const char* soundEffectFilePath)
{
    m_pSounds.emplace(soundEffectFilePath, std::make_unique<BrokkrSound>(soundEffectFilePath));
}

bool Brokkr::AudioManager::PlaySoundEffect(const std::string& effectName, int loops)
{
    if (Mix_Chunk* chunk = m_pSounds[effectName]->m_sound) 
    {
        Mix_PlayChannel(-1, chunk, loops);
        return true;
    }
    //
    return false;
}

bool Brokkr::AudioManager::PlayMusic(const std::string& musicName, int loops)
{
    if (Mix_Music* music = m_pMusic[musicName]->m_music) 
    {
        Mix_PlayMusic(music, loops);
        return true;
    }
    //
    return false;
}

void Brokkr::AudioManager::Destroy()
{
    for (const auto& music : m_pMusic) 
    {
        music.second->Destroy();
    }
    m_pMusic.clear();

    for (const auto& sound : m_pSounds) 
    {
        sound.second->Destroy();
    }
    m_pSounds.clear();
    CloseAudio();
}
