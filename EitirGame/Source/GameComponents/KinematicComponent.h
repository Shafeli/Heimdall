#pragma once
#include "Entity/GameEntity/Component/Component.h"
#include <Vector2.h>

namespace Brokkr
{
    class GameEntity;
    class CoreSystems;
}

class KinematicComponent final : public Brokkr::Component
{
    //position: Vec3
    //orientation : float  // probably radians

    Brokkr::GameEntity* m_pOwner = nullptr;
    Brokkr::CoreSystems* m_pCoreSystems = nullptr;

    Brokkr::Vector2<float> m_position;
    Brokkr::Vector2<float> m_velocity;
    Brokkr::Vector2<float> m_acceleration;

    float m_orientation; // Radians
    float m_angularVelocity;
    float m_angularAcceleration;

    float m_maxSpeed;
    float m_maxAcceleration;
    float m_maxAngularVelocity;
    float m_maxAngularAcceleration;


public:
    KinematicComponent
    (
        Brokkr::GameEntity* pOwner,
        Brokkr::CoreSystems* pCoreSystems,
        float maxSpeed,
        float maxAcceleration,
        float maxAngVel,
        float maxAngAcceleration
    )
        : m_pOwner(pOwner)
        , m_pCoreSystems(pCoreSystems)
        , m_velocity(0, 0)
        , m_acceleration(0, 0)
        , m_orientation(0)
        , m_angularVelocity(0)
        , m_angularAcceleration(0)
        , m_maxSpeed(maxSpeed)
        , m_maxAcceleration(maxAcceleration)
        , m_maxAngularVelocity(maxAngVel)
        , m_maxAngularAcceleration(maxAngAcceleration)
    {
        
    }

    virtual bool Init() override;
    virtual void Update() override;
    virtual void Destroy() override;
    virtual void LateUpdate() override;

    void SetLinearAcceleration(const Brokkr::Vector2<float>& newAcceleration);
    void SetAngularAcceleration(float newAngularAcceleration);
    void Stop();

    [[nodiscard]] Brokkr::Vector2<float> GetVelocity() const { return m_velocity; }
    [[nodiscard]] Brokkr::Vector2<float> GetPosition() const { return m_position; }
private:

    void ClampVelocity();
    void ClampAngularVelocity();
};

/*

class Kinematic
    // current values
    &currLocation: Location  // reference; not owned by us
    velocity: Vec3  // direction of movement and speed
    acceleration: Vec3
    angularVelocity: float  // radians per second
    angularAcceleration: float

    // max values (tuned)
    maxSpeed: float  // can also be set by gameplay
    maxAcceleration: float
    maxAngularVelocity: float
    maxcAngularAcceleration: float

    function UpdateLocation(deltaTime)
        // update position
        currLocation.position += velocity * deltaTime

        // update velocity based on acceleration
        velocity += acceleration * deltaTime

        // clamp velocity
        if velocity.Length() > maxSpeed
            velocity = velocity.Normalized() * maxSpeed

        // update orientation
        currLocation.orientation += angularVelocity * deltaTime
        currLocation.orientation = std::fmod(orientation, 2 * PI)  // wrap orientation so that 0 <= orientation <= 2PI

        // update angular velocity based on acceleration
        angularVelocity += angularAcceleration

        // clamp
        if angularVelocity > maxAngularVelocity
            angularVelocity = maxAngularVelocity

        // If you're using another engine, teleport here.  i.e.
        // agent.gameObject.transformComponent.Position = new Vector3(currLocation.position.x, currLocation.position.y, currLocation.position.z)
        // same with orientation

    // Note: For these setter functions, you need to take into account negative values.  The easy solution to
    // interpret the max values as absolute values.  So the actual range would be from -maxAcceleration to
    // maxAcceleration.
    function SetLinearAcceleration(newLinearAcceleration)
        clamp newLinearAcceleration to maxAcceleration
        acceleration = newLinearAcceleration

    function SetAngularAcceleration(newAngularAcceleration)
        clamp newAngularAcceleration to maxAcceleration
        angularAcceleration = newAngularAcceleration

    function Stop()
        velocity = (0,0,0)
        angularVelocity = 0
        acceleration = (0,0,0)
        angularAcceleration = 0



*/