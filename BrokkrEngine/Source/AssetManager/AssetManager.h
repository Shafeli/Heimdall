#pragma once
#include <memory>
#include <vector>

#include "Core/Core.h"

/////////////////////////////////////////////////
//          AssetManger
//  TODO: Docs
//  
/////////////////////////////////////////////////

namespace Brokkr
{
    class SDLWindow;
    class SDLRenderer;
    class AssetManager;

    class AssetSubsystem
    {
    public:

        AssetSubsystem(AssetManager* assetManager) : m_pAssetManager(assetManager) {}
        virtual ~AssetSubsystem() = default;

        virtual void Destroy() = 0;

    protected:
        AssetManager* m_pAssetManager;
    };

    class CoreSystems;

    class AssetManager final : public System
    {
        // inline static Logger m_fileLog{ "AssetSystemManagerLog" };
        std::vector<std::unique_ptr<AssetSubsystem>> m_pAssetSystems{ };

    public:
        explicit AssetManager(CoreSystems* pCoreManager)
            : System(pCoreManager)
        {
            //
        }

        void Init(SDLWindow* window, SDLRenderer* renderer);

        template <typename AssetSystem>
        AssetSystem* GetAssetSystem()
        {
            // Iterate through all components in the vector
            for (auto& component : m_pAssetSystems)
            {
                // If the cast is successful
                if (AssetSystem* target = dynamic_cast<AssetSystem*>(component.get()))
                {
                    return target;
                }
            }
            return nullptr; // If no system of type is found
        }

        template <typename AssetSystem, typename ... Args>
        AssetSystem* AddAssetSystem(Args&&... args)
        {
            // Create a instance of the component type passing in the current AssetSystem pointer
            std::unique_ptr<AssetSystem> newComponent = std::make_unique<AssetSystem>(this, std::forward<Args>(args)...);

            AssetSystem* result = newComponent.get(); // Get a raw pointer to the component

            m_pAssetSystems.emplace_back(std::move(newComponent)); // Add the system to the vector 
            return result; // Return a pointer
        }

        virtual void Destroy() override;
    };

}