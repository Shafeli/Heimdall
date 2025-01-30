#include "BrokkrMusic.h"

#include <SDL_mixer.h>

BrokkrMusic::BrokkrMusic(const std::string& filename)
{
    if (Mix_Music* music = Mix_LoadMUS(filename.c_str())) 
    {
        m_music = music;
    }
    else 
    {
        // logg
    }
}

void BrokkrMusic::Destroy()
{
    if (m_music)
    {
        Mix_FreeMusic(m_music);
        m_music = nullptr;
    }
}

Mix_Music* BrokkrMusic::GetMusicChunk() const
{
    return m_music;
}
