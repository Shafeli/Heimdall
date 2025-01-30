#pragma once
#include "../../AssetManager/AssetManager.h"

namespace Brokkr
{
    class ArchiveManager final : public AssetSubsystem
    {
        // std::unordered_map<const char*, std::unique_ptr<FolderAssistant>> m_assetArchives;

    public:
        ArchiveManager(AssetManager* assetManager)
        :AssetSubsystem(assetManager)
        {
            //
        }

        void LoadFolder(const char* filePath); // add a folder to the Archive Manager

        char* GetAssetData(const char* assetName); // Searches for an asset with the given name

        // Searches for an asset with the given name & file name for speed
        char* GetAssetData(const char* fileName, const char* assetName);
        virtual void Destroy() override;
    };
}


