#include "SeekSteeringBehavior.h"

Brokkr::Vector2<float> SeekSteeringBehavior::GetSteeringForce() const
{
    const Brokkr::ColliderComponent* pAiController = m_pOwner->GetLocation();
    if (!pAiController)
        return {}; // Early return if no controller is found

    Brokkr::Vector2 aiPosition = pAiController->GetTransform().GetPosition();

    Brokkr::Vector2 direction = targetPosition - aiPosition;
    float distance = direction.Length();

    // If the distance is very small, return no steering force (or you can snap to the target)
    if (distance < 1.0f)  // You can adjust the threshold here
    {
        return {};
    }

    // Normalize direction to get a unit vector
    Brokkr::Vector2 normdir = direction.Normalize();
    return normdir;
}

