#include "TransformComponent.h"

#include <tinyxml2.h>

#include "ColliderComponent.h"
#include "GameEntity.h"
#include "Core/EngineDefinitions.h"
#include "EventManager/EventManager.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

#define DEBUG_RENDER 0

Brokkr::TransformComponent::TransformComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rectangle<float> transform)
    : m_systemRef(pCoreSystems)
    , m_pOwner(pOwner)
    
{
    m_transform = transform;
    m_pEventManager = pCoreSystems->GetCoreSystem<EventManager>();
}

bool Brokkr::TransformComponent::Init()
{
    m_transform.MoveTo(m_startPos);

    return true; //default
}

void Brokkr::TransformComponent::Update()
{
    // not needed for this component for now
}

void Brokkr::TransformComponent::Destroy()
{

}

void Brokkr::TransformComponent::AddCollider(ColliderComponent* pColliderComponent)
{
    m_collider = pColliderComponent;

    const std::string eventStr = EngineDefinitions::UPDATE_TRANSFORM_POSITION_EVENT + std::to_string(m_pOwner->GetId());

    m_updateHandler.first = Event::kPriorityNormal; // update before render data is pushed
    m_updateHandler.second = [this](auto&& event) { UpdatePosition(std::forward<decltype(event)>(event)); };
    m_pEventManager->AddHandler(eventStr.c_str(), m_updateHandler);

}

void Brokkr::TransformComponent::UpdatePosition([[maybe_unused]] const Event& event)
{
    if (m_collider)
    {
        m_transform.MoveTo(m_collider->GetTransform().GetPosition());
    }
}

// TODO: Finish Resize in Entity Components
void Brokkr::TransformComponent::Resize([[maybe_unused]] float width, [[maybe_unused]] float height)
{
    /*m_transform->m_collider.Resize(width, height);*/
}

void Brokkr::TransformComponent::RegisterCreationFunction(EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("TransformComponent", CreateComponent);
}

void Brokkr::TransformComponent::CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element,
                                                          CoreSystems* coreSystems)
{
    float x = element->FloatAttribute("x");
    float y = element->FloatAttribute("y");
    float width = element->FloatAttribute("width");
    float height = element->FloatAttribute("height");

    // create TransformComponent using x, y, width, height
    auto component = entity->AddComponent<TransformComponent>
        (
            coreSystems
            , Brokkr::Rectangle<float>({ x, y }, { width, height })
            );
    component->SetStartingPos({ x,y });
}


