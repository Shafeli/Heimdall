#include "AssetManager.h"

#include "2DRendering/SDLRenderer.h"
#include "2DRendering/SDLWindow.h"
#include "ArchiveManager/ArchiveManager.h"
#include "AudioManager/AudioManager.h"
#include "FontManager/FontManager.h"
#include "TextureManager/TextureManager.h"
#include "XMLManager/XMLManager.h"

void Brokkr::AssetManager::Init()
{

    const auto pWindow = m_pCoreManager->GetCoreSystem<SDLWindow>();
    const auto pRender = m_pCoreManager->GetCoreSystem<SDLRenderer>();

    const auto pArchiveManager = AddAssetSystem<ArchiveManager>();
    if (!pArchiveManager)
    {
        const std::string error = "Error Asset System Failed : ArchiveManager did not Construct!";
    }

    const auto pTextureManager = AddAssetSystem<TextureManager>();
    if (!pTextureManager)
    {
        const std::string error = "Error Asset System Failed : TextureManager did not Construct!";
    }

    const auto pXMLManager = AddAssetSystem<XMLManager>();
    if (!pXMLManager)
    {
        const std::string error = "Error Asset System Failed : XMLManager did not Construct!";
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
    pXMLManager->Init(m_pCoreManager);
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
