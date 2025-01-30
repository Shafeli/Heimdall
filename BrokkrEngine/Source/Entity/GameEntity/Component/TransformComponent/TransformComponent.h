#pragma once
#include <ColliderComponent.h>
#include <Component.h>
#include <EventManager/EventManager.h>

/*#define BROKKR_IGNORE_COORDINATE_Y 1
#define BROKKR_IGNORE_COORDINATE_X 2
#define BROKKR_IGNORE_NONE 3*/

namespace Brokkr
{
    class Event;
    class EventManager;
    class EntityXMLParser;
    class ColliderComponent;
    class GameEntity;

    class TransformComponent final : public Component
    {
        Rectangle<float> m_transform;

        CoreSystems* m_systemRef;
        ColliderComponent* m_collider = nullptr;
        GameEntity* m_pOwner = nullptr;
        EventManager* m_pEventManager = nullptr;

        EventManager::EventHandler m_updateHandler;

        Vector2<float> m_startPos;

    public:
        TransformComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rectangle<float> transform);

        virtual bool Init() override;
        virtual void Update() override;
        virtual void Destroy() override;
        virtual void Render() override;

        void SetStartingPos(Vector2<float> newStart) { m_startPos = newStart; }

        [[nodiscard]] Rectangle<float> GetTransform() const
        {
            return m_transform;
        }
        [[nodiscard]] Vector2<float> GetStartingPos() const { return m_startPos; }

        void MoveTo(Vector2<float> newPos)
        {
  
            m_transform.MoveTo(newPos);
            const std::string eventStr = "UpdatePosition" + std::to_string(m_pOwner->GetId());
            const Event outEvent = Event::EventType(eventStr.c_str(), Event::kPriorityNormal);
            m_pEventManager->PushEvent(outEvent);
        }

        void AddCollider(ColliderComponent* pColliderComponent);

        // When ever Physics updates the final displacement for the frame update this after
        void UpdatePosition([[maybe_unused]] const Event& event);

        void Resize(float width, float height);

        // Static registration function
        static void RegisterCreationFunction(EntityXMLParser* parser);
        static void CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element, CoreSystems* coreSystems);

    };
}
