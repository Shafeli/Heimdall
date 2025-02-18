#pragma once
#include <Vector2.h>
#include "Entity/GameEntity/Component/TransformComponent/TransformComponent.h"


class AgentController;

class AgentSteeringBehavior
{

public:
    explicit AgentSteeringBehavior(AgentController* pOwner)
        : m_pOwner(pOwner)
    {
        //
    }

    virtual ~AgentSteeringBehavior() = default;
    virtual void Update(float deltaTime) = 0;

    // Steering behavior
    [[nodiscard]] virtual Brokkr::Vector2<float> GetSteeringForce() const = 0;

protected:
    AgentController* m_pOwner = nullptr;
};
