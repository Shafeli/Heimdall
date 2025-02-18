#pragma once

#include "AgentController.h"
#include "AgentSteeringBehavior.h"

class SeekSteeringBehavior final : public AgentSteeringBehavior
{
    Brokkr::Vector2<float> targetPosition;  // Target to move toward
    Brokkr::Vector2<float> m_steeringForce;
public:
    explicit SeekSteeringBehavior(AgentController* pOwner, const Brokkr::Vector2<float>& target)
        : AgentSteeringBehavior(pOwner)
        , targetPosition(target)
    {
    }

    virtual void Update(float deltaTime) override
    {
        if (m_pOwner->GetLocation()->GetTransform().Contains(targetPosition))
        {
            m_pOwner->Stop();
            m_pOwner->FindPlayer();
            return;
        }

        m_pOwner->ApplyForce(GetSteeringForce());
    }


    [[nodiscard]] virtual Brokkr::Vector2<float> GetSteeringForce() const override;
    virtual ~SeekSteeringBehavior() override = default;
};
