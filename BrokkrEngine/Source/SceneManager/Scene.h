#pragma once

#include "EventManager/EventManager.h"

/////////////////////////////////////////////////
// State interface
/////////////////////////////////////////////////

//TODO: Idea: Adding a clock to time the amount of seconds on this state

namespace Brokkr
{

    class CoreSystems;
    class GameEntityManager;

    class Scene
    {

    public:
        explicit Scene(CoreSystems* pEngineSystems)
            :m_pCoreSystems(pEngineSystems)
        {
            // What needs to be initialized
        }

        /////////////////////////////////////////////////
        // override state methods
        /////////////////////////////////////////////////
        virtual ~Scene() = default;
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Exit() = 0;
        virtual void Enter() = 0;
        virtual void Destroy() = 0;

    protected:

        // System handles for easy access to things that are commonly used
        CoreSystems* m_pCoreSystems = nullptr;

    };
}