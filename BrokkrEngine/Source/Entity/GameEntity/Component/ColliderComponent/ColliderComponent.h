#pragma once
#include "GameEntity.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "2DPhysicsManager/PhysicsManager.h"
#include "Core/Core.h"
#include "EventManager/EventManager.h"


namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class EntityXMLParser;
    class PhysicsManager;
    class Collider;
    class TransformComponent;

    class ColliderComponent final : public Component
    {
        Collider* m_transform;

        GameEntity* m_pOwner = nullptr;
        PhysicsManager* m_pPhysicsManager = nullptr;
        EventManager* m_pEventManager = nullptr;

        EventManager::EventHandler m_blockedHandler;
        EventManager::EventHandler m_onEnterHandler;

        std::string m_eventStr;
        Event m_blockEvent;

        bool m_isPassable = true;
        Rectangle<float> m_transformStart{};
        int m_overlapType = -1;

    public:

        ColliderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rectangle<float> transform, int overlap, bool passable);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render() override;

        [[nodiscard]] Rectangle<float> GetTransform() const;

        void BlockMove([[maybe_unused]] const Event& event);

        void AdjustByCurrentPos(float x, float y);
        void AdjustByPos(const Vector2<float>& newPos);
        void AdjustBy(float x, float y);
        void AbsoluteMove(Vector2<float> pos) { m_pPhysicsManager->AbsoluteMove(m_transform, pos); }

        // Static registration function
        static void RegisterCreationFunction(EntityXMLParser* parser);
        static void CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element, CoreSystems* coreSystems);
        virtual void Enable() override;
        virtual void Disable() override;
    };
}

