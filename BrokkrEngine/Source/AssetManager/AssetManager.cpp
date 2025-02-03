#include "AssetManager.h"

#include "2DRendering/SDLRenderer.h"
#include "2DRendering/SDLWindow.h"
#include "ArchiveManager/ArchiveManager.h"
#include "AudioManager/AudioManager.h"
#include "FontManager/FontManager.h"
#include "2DTextureManager/TextureManager2D.h"


void Brokkr::AssetManager::Init(SDLWindow* window, SDLRenderer* renderer)
{

    const auto pWindow = window;
    const auto pRender = renderer;

    const auto pArchiveManager = AddAssetSystem<ArchiveManager>();
    if (!pArchiveManager)
    {
        const std::string error = "Error Asset System Failed : ArchiveManager did not Construct!";
    }

    const auto pTextureManager = AddAssetSystem<TextureManager2D>();
    if (!pTextureManager)
    {
        const std::string error = "Error Asset System Failed : TextureManager2D did not Construct!";
    }

    const auto pFontManager = AddAssetSystem<FontManager>();
    if (!pFontManager)
    {
        const std::string error = "Error Asset System Failed : Font Manager did not Construct!";
    }

    const auto pAudioManager = AddAssetSystem<AudioManager>();
    if (!pAudioManager)
    {
        const std::string error = "Error Asset System Failed : AudioManager did not Construct!";
    }

    pTextureManager->Init(pWindow, pRender);
    pFontManager->Init(pRender);
}

void Brokkr::AssetManager::Destroy()
{
    //
    const auto pAudioManager = GetAssetSystem<AudioManager>();
    if (pAudioManager)
    {
        pAudioManager->Destroy();
    }
}
