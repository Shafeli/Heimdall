#include "AgentController.h"

#include "SeekSteeringBehavior.h"
#include "Tinyxml2.h"
#include "../KinematicComponent.h"
#include "Entity/GameEntityManager/GameEntityManager.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

void AgentController::ApplyForce(Brokkr::Vector2<float> steeringForce)
{
    if (kinematic)
    {

        Brokkr::Vector2<float> acceleration = steeringForce * 2.5f;    // can Calculate acceleration by dividing the force by mass

        kinematic->SetLinearAcceleration(acceleration);
    }
}

void AgentController::MakeDecision()
{
}

void AgentController::Stop()
{
    kinematic->Stop();
}

bool AgentController::Init()
{
    location = m_pOwner->GetComponent<Brokkr::ColliderComponent>();
    kinematic = m_pOwner->AddComponent<KinematicComponent>(m_pCoreSystems, maxSpeed, maxAcceleration, maxAngVel, maxAngAcceleration);

    Brokkr::GameEntity* target = m_pCoreSystems->GetCoreSystem<Brokkr::GameEntityManager>()->GetEntityByName(m_targetName);
    Brokkr::Vector2<float> targetPos = target->GetComponent<Brokkr::TransformComponent>()->GetTransform().GetPosition();

    auto tempHolder = std::make_unique<SeekSteeringBehavior>(this, targetPos);
    steeringBehavior = std::move(tempHolder); 


    return true;
}

void AgentController::Update()
{
    MakeDecision();

    if (steeringBehavior)
    {
        
        steeringBehavior->Update((float)m_pCoreSystems->GetDeltaTime());
    }
}

void AgentController::Destroy()
{
}

void AgentController::NullSteering()
{
    steeringBehavior = nullptr;
}

void AgentController::FindPlayer()
{
    Brokkr::GameEntity* target = m_pCoreSystems->GetCoreSystem<Brokkr::GameEntityManager>()->GetEntityByName("Player");
    Brokkr::Vector2<float> targetPos = target->GetComponent<Brokkr::TransformComponent>()->GetTransform().GetPosition();

    auto tempHolder = std::make_unique<SeekSteeringBehavior>(this, targetPos);
    steeringBehavior = std::move(tempHolder);
}

void AgentController::RegisterCreationFunction(Brokkr::EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("AgentController", CreateComponent);
}

void AgentController::CreateComponent(Brokkr::GameEntity* entity, tinyxml2::XMLElement* element,
    Brokkr::CoreSystems* coreSystems)
{
    const char* targetOne = element->Attribute("Target");
    float mSpeed = element->FloatAttribute("MaxSpeed");
    float mAcceleration = element->FloatAttribute("MaxAcceleration");
    float mAngVel = element->FloatAttribute("MaxAngVel");
    float mAndAcceleration = element->FloatAttribute("MaxAngAcceleration");

    // create SpriteComponent using texture name
    entity->AddComponent<AgentController>(coreSystems, targetOne,  mSpeed * 100.0f, mAcceleration * 100.0f, mAngVel * 100.0f, mAndAcceleration * 100.0f);
}
