#pragma once
#include <string>

namespace Brokkr
{
    class AudioManager;
}

struct _Mix_Music;

class BrokkrMusic
{
    friend Brokkr::AudioManager;
    _Mix_Music* m_music = nullptr;

public:
    explicit BrokkrMusic(const std::string& filename);
    void Destroy();

private:
    [[nodiscard]] _Mix_Music* GetMusicChunk() const;
};

