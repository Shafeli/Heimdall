#include "BrokkrSound.h"

#include <SDL_mixer.h>

BrokkrSound::BrokkrSound(const std::string& filename)
{
    if (Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str())) 
    {
        m_sound = chunk;
    }
    else 
    {

    }
}

void BrokkrSound::Destroy()
{
    if (m_sound)
    {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

Mix_Chunk* BrokkrSound::GetSoundChunk() const
{
    return m_sound;
}
