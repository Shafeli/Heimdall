#include "KinematicComponent.h"

#include <iostream>

#include "GameComponentReg.h"
#include "Core/Core.h"
#include "Entity/GameEntity/GameEntity.h"

bool KinematicComponent::Init()
{

    return true;
}

void KinematicComponent::Update()
{
    const auto Collider = m_pOwner->GetComponent<Brokkr::ColliderComponent>();
    m_position = Collider->GetTransform().GetPosition();

    const float dt = (float)m_pCoreSystems->GetDeltaTime();

    // Update linear motion
    m_velocity += m_acceleration * dt;
    ClampVelocity(); // Ensure reasonable velocity

    // Update position
    m_position += m_velocity * dt;

    // Update angular motion
    m_angularVelocity += m_angularAcceleration * dt;
    ClampAngularVelocity();
    m_orientation += m_angularVelocity * dt;
}

void KinematicComponent::Destroy()
{
}

void KinematicComponent::LateUpdate()
{
    Component::LateUpdate();
    auto Collider = m_pOwner->GetComponent<Brokkr::ColliderComponent>();

    Collider->AbsoluteMove(m_position);
}

void KinematicComponent::SetLinearAcceleration(const Brokkr::Vector2<float>& newAcceleration)
{

    if (newAcceleration.Length() > m_maxAcceleration)
    {
        Brokkr::Vector2<float> nA = newAcceleration;

        m_acceleration = m_maxAcceleration * nA.Normalize();
    }
    else
    {
        m_acceleration = newAcceleration;
    }
}

void KinematicComponent::SetAngularAcceleration(float newAngularAcceleration)
{
    // If the new value exceeds the max clamp
    if (newAngularAcceleration > m_maxAngularAcceleration)
    {
        m_angularAcceleration = m_maxAngularAcceleration;
    }

    // If the new value is less than the negative max inverse maximum
    else if (newAngularAcceleration < -m_maxAngularAcceleration)
    {
        m_angularAcceleration = -m_maxAngularAcceleration;
    }
    else
    {
        m_angularAcceleration = newAngularAcceleration; // no Clamp
    }
}

void KinematicComponent::Stop()
{
    m_velocity = { 0.0f, 0.0f };
    m_acceleration = { 0.0f, 0.0f };
    m_angularVelocity = 0.0f;
    m_angularAcceleration = 0.0f;
}

void KinematicComponent::ClampVelocity()
{
    if (const float speed = m_velocity.Length(); speed > m_maxSpeed)
    {
        m_velocity = m_velocity.GetClampedLength(m_maxSpeed);
    }
}

void KinematicComponent::ClampAngularVelocity()
{
    // Angular velocity
    Brokkr::Vector2<float> angularVelocityVector(m_angularVelocity, 0.0f);

    // Clamp
    angularVelocityVector = angularVelocityVector.GetClampedLength(m_maxAngularVelocity);
    m_angularVelocity = angularVelocityVector.m_x; 
}
