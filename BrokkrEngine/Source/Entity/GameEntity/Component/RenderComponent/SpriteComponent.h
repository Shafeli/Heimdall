#pragma once
#include <Component.h>
#include <string>
#include "GameEntity.h"
#include "Core/Core.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class SDLRenderer;
    class SDLWindowSystem;
    class TextureManager2D;
    class EntityXMLParser;
    class Texture2D;
    class GameEntity;
    class TransformComponent;

    class SpriteComponent final : public Component
    {
        GameEntity* m_pOwner = nullptr;
        CoreSystems* m_pCoreSystems = nullptr;
        TextureManager2D* m_pTextureManager = nullptr;
        Texture2D* m_texture;
        SDLWindowSystem* m_pSDLWindowSystem = nullptr;
        SDLRenderer* m_pSDLRenderer = nullptr;
        TransformComponent* m_pTransformComponent = nullptr;

        std::string m_textureName;
        bool m_centerTransform;
        bool m_active;

    public:
        SpriteComponent
        (
            GameEntity* pOwner,
            CoreSystems* systemRef,
            const std::string& textureName,
            bool center
        );

        [[nodiscard]] const char* GetTextureName() const { return m_textureName.c_str(); }

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render();

        virtual void Attach() override;
        virtual void Detach() override;
        virtual void Enable() override;
        virtual void Disable() override;

        void CenterToTransform() { m_centerTransform = true; }
        void CenterToTransformOff() { m_centerTransform = false; }

        // Static registration function
        static void RegisterCreationFunction(EntityXMLParser* parser);
        static void CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element, CoreSystems* coreSystems);
    };
}
