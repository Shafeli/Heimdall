#include "ColliderComponent.h"
#include <string>
#include <Tinyxml2.h>

#include "GameEntity.h"
#include "TransformComponent.h"

#include "EventManager/Event/PayloadComponent/CollisionPayload/CollisionPayload.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

/*
#define BROKKR_OVERLAP_STATIC 0
#define BROKKR_OVERLAP_DYNAMIC 1
#define BROKKR_OVERLAP_ALL 2
*/

#define DEBUG_RENDER 0

Brokkr::ColliderComponent::ColliderComponent(GameEntity* pOwner, CoreSystems* pCoreSystems, Rectangle<float> transform, int overlap, bool passable)
    : m_pOwner(pOwner)
    , m_eventStr("Blocked")
    , m_blockEvent(Event::EventType("Block", Event::kPriorityNormal))
    , m_isPassable(passable)
    , m_transformStart(transform)
    , m_overlapType(overlap)
{
    m_pPhysicsManager = pCoreSystems->GetCoreSystem<PhysicsManager>();
    m_pEventManager = pCoreSystems->GetCoreSystem<EventManager>();
}

bool Brokkr::ColliderComponent::Init()
{

    if (!m_isPassable)
    {
        const auto eventStr = "OnEnter" + std::to_string(m_pOwner->GetId());
        m_onEnterHandler.first = Event::kPriorityNormal;
        m_onEnterHandler.second = [this](auto&& event) { BlockMove(std::forward<decltype(event)>(event)); };
        m_pEventManager->AddHandler(eventStr.c_str(), m_onEnterHandler);
    }

    const auto newPos = m_pOwner->GetComponent<TransformComponent>()->GetTransform().GetPosition();
    m_transformStart.MoveTo(newPos);

    // During init recenter the transform 
    if (m_overlapType == 1)
    {
        m_transform = m_pPhysicsManager->CreateCollider(m_transformStart, m_pOwner->GetId(), true, m_overlapType);
    }
    else if (m_overlapType == 0)
    {
        m_transform = m_pPhysicsManager->CreateCollider(m_transformStart, m_pOwner->GetId(), false, m_overlapType);
    }
    else if (m_overlapType == 2)
    {
        m_transform = m_pPhysicsManager->CreateCollider(m_transformStart, m_pOwner->GetId(), true, m_overlapType);
    }

    return true; //default
}

Brokkr::Rectangle<float> Brokkr::ColliderComponent::GetTransform() const
{
    if (m_transform)
        return m_transform->m_collider;
    return {};
}

void Brokkr::ColliderComponent::Update()
{
    // not needed for this component for now

    if (m_transform)
    {
        AdjustByPos(m_transform->m_collider.GetPosition());
    }

}

void Brokkr::ColliderComponent::Destroy()
{
    m_pPhysicsManager->Remove(m_transform);
}

void Brokkr::ColliderComponent::Render()
{
    Component::Render();

#if DEBUG_RENDER
    //auto colorHax = BasicColor();
    //colorHax.RandomColors();
    //m_pRenderPipeline->QueueToRender(GetTransform(), colorHax, 0.0f);
#endif

}

void Brokkr::ColliderComponent::BlockMove(const Event& event)
{
    CollisionPayload* data = event.GetComponent<CollisionPayload>();

    // Check if the object being collided with is the blocking collider
    if (data->GetObjectMoving()->m_ownerID == m_pOwner->GetId())
    {
        return;
    }

    PhysicsManager::RequestMoveCorrection(data->GetObjectMoving(), -data->GetMovingObjectsDisplacement());
}

void Brokkr::ColliderComponent::AdjustByCurrentPos(float x, float y)
{
    auto currentPos = m_pOwner->GetComponent<TransformComponent>()->GetTransform().GetPosition();

    currentPos.m_x += x;
    currentPos.m_y += y;

    m_pPhysicsManager->AbsoluteMove(m_transform, currentPos);
}

void Brokkr::ColliderComponent::AdjustByPos(const Vector2<float>& newPos)
{
    // Get the current position
    Vector2<float> currentPos = GetTransform().GetPosition();

    // delta (new position - current position)
    Vector2<float> delta = newPos - currentPos;


    AdjustBy(delta.m_x, delta.m_y);
}

void Brokkr::ColliderComponent::AdjustBy(float x, float y)
{

    m_pPhysicsManager->RequestMove(m_transform, { x, y });
}

void Brokkr::ColliderComponent::RegisterCreationFunction(EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("ColliderComponent", CreateComponent);
}

void Brokkr::ColliderComponent::CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element,
    CoreSystems* coreSystems)
{
    const char* overlap = element->Attribute("overlap");
    int overlapConvert = 0;
    if (overlap != nullptr)
    {
        if (strcmp(overlap, "dynamic") == 0)
        {
            overlapConvert = BROKKR_OVERLAP_DYNAMIC;
        }
        else if (strcmp(overlap, "static") == 0)
        {
            overlapConvert = BROKKR_OVERLAP_STATIC;
        }
        else if (strcmp(overlap, "all") == 0)
        {
            overlapConvert = BROKKR_OVERLAP_ALL;
        }
    }

    const char* passable = element->Attribute("passable");
    bool moveConvert = false;
    if (passable != nullptr)
    {
        if (strcmp(passable, "yes") == 0)
        {
            moveConvert = true;
        }
        else if (strcmp(passable, "no") == 0)
        {
            moveConvert = false;
        }
    }

    const auto transformComponent = entity->GetComponent<TransformComponent>();

    const auto hax = entity->AddComponent<ColliderComponent>(coreSystems, transformComponent->GetTransform(), overlapConvert, moveConvert);

    transformComponent->AddCollider(hax);
}

void Brokkr::ColliderComponent::Enable()
{
    Component::Enable();
}

void Brokkr::ColliderComponent::Disable()
{
    Component::Disable();
}


