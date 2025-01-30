#include "ArchiveManager.h"

void Brokkr::ArchiveManager::LoadFolder(const char* filePath)
{
    // Convert input char* to string
    const std::string str_path(filePath);

    // Find position of last '/' character and extract filename from string
    const std::string filename = str_path.substr(str_path.find_last_of("/") + 1);

    // Check if folder with the same name already exists
}

char* Brokkr::ArchiveManager::GetAssetData(const char* assetName)
{
    // Iterate over all FolderAssistant objects in the unordered_map

    // file is not found

    return nullptr;
}

char* Brokkr::ArchiveManager::GetAssetData(const char* fileName, const char* assetName)
{
    // return m_assetArchives[fileName].get()->GetDecompressedData(assetName);
    return nullptr;
}

void Brokkr::ArchiveManager::Destroy()
{
    
}
