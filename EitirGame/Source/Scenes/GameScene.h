#pragma once

#include "SceneManager/Scene.h"

namespace Brokkr
{
    class PhysicsManager;
    class XMLManager;
}

class GameScene : public Brokkr::Scene
{
    Brokkr::XMLManager* m_pXmlManager = nullptr;
    Brokkr::GameEntityManager* m_pEntityManager = nullptr;
    Brokkr::PhysicsManager* m_pPhysicsManager = nullptr;

public:
    explicit GameScene(Brokkr::CoreSystems* pEngineSystems)
        : Scene(pEngineSystems)
    {
    }

    virtual void Init() override;
    virtual void Update() override;
    virtual void Exit() override;
    virtual void Enter() override;
    virtual void Destroy() override;
};

