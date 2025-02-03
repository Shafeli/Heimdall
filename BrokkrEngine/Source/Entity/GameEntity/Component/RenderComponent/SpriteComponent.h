#pragma once
#include <Component.h>
#include <string>
#include <EventManager/EventManager.h>
#include "GameEntity.h"
#include "AssetManager/AssetManager.h"
#include "Core/Core.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class EntityXMLParser;
    class Texture2D;
    class GameEntity;

    class SpriteComponent final : public Component
    {

        GameEntity* m_pOwner = nullptr;
        CoreSystems* m_systemRef;

        std::string m_textureName;


        EventManager* m_pEventManager = nullptr;
        AssetManager* m_pAssetManager = nullptr;

        Texture2D* m_texture = nullptr;

        float m_depth;
        bool m_active;
        bool m_centerTransform;
        std::string m_textureBlendColor;

    public:
        SpriteComponent
        (
            GameEntity* pOwner,
            const std::string& textureName,
            CoreSystems* systemRef,
            float depth,
            bool active,
            bool center,
            const std::string& textureBlendColor
        );

        [[nodiscard]] const char* GetTextureName() const { return m_textureName.c_str(); }

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render() override;

        virtual void Attach() override;
        virtual void Detach() override;
        virtual void Enable() override;
        virtual void Disable() override;

        void CenterToTransform() { m_centerTransform = true; }
        void CenterToTransformOff() { m_centerTransform = false; }

        void SetBlendColor();
        void SetColor(const std::string& color);
        void ResetColor();

        void SetTextureName(const std::string& textureName)
        {
            m_textureName.clear();
            m_textureName = textureName;
        }

        // Static registration function
        static void RegisterCreationFunction(EntityXMLParser* parser);
        static void CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element, CoreSystems* coreSystems);
    };
}
