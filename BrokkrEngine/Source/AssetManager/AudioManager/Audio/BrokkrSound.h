#pragma once
#include <string>

namespace Brokkr
{
    class AudioManager;
}

struct Mix_Chunk;

class BrokkrSound
{
    friend Brokkr::AudioManager;
    Mix_Chunk* m_sound = nullptr;

public:
    BrokkrSound(const std::string& filename);

    void Destroy();

private:
    Mix_Chunk* GetSoundChunk() const;
};
