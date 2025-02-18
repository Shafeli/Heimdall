#pragma once
#include <memory>

#include "AgentSteeringBehavior.h"
#include "Entity/GameEntity/GameEntity.h"
#include "Entity/GameEntity/Component/Component.h"
#include "Entity/GameEntity/Component/ColliderComponent/ColliderComponent.h"

class KinematicComponent;

class AgentController final : public Brokkr::Component
{
    // Ownership of components
    Brokkr::GameEntity* m_pOwner = nullptr;
    Brokkr::CoreSystems* m_pCoreSystems = nullptr;

    // Location location;
    Brokkr::ColliderComponent* location = nullptr;
    KinematicComponent* kinematic = nullptr;              
    std::unique_ptr<AgentSteeringBehavior> steeringBehavior = nullptr;
    std::string m_targetName;

    // Max
    float maxSpeed;
    float maxAcceleration;
    float maxAngVel;
    float maxAngAcceleration;

public:
    AgentController
    (
        Brokkr::GameEntity* pOwner,
        Brokkr::CoreSystems* pCoreSystems,
        const std::string& targetName,
        float mSpeed,
        float mAcceleration,
        float mAngVel,
        float mAndAcceleration
    )
        : m_pOwner(pOwner)
        , m_pCoreSystems(pCoreSystems)
        , m_targetName(targetName)
        , maxSpeed(mSpeed)
        , maxAcceleration(mAcceleration)
        , maxAngVel(mAngVel)
        , maxAngAcceleration(mAndAcceleration)
    {
        
    }

    [[nodiscard]] Brokkr::ColliderComponent* GetLocation() const { return location; }

    void ApplyForce(Brokkr::Vector2<float> steeringForce);
    // Makes decisions based on perceps
    void MakeDecision();

    // Stop the agent's movement
    void Stop();
    virtual bool Init() override;
    virtual void Update() override;
    virtual void Destroy() override;
    void NullSteering();
    void FindPlayer();


    // Static registration function
    static void RegisterCreationFunction(Brokkr::EntityXMLParser* parser);
    static void CreateComponent(Brokkr::GameEntity* entity, tinyxml2::XMLElement* element, Brokkr::CoreSystems* coreSystems);
};
