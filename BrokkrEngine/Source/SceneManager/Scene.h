#pragma once

#include "EventManager/EventManager.h"

/////////////////////////////////////////////////
// State interface
/////////////////////////////////////////////////

//TODO: Idea: Adding a clock to time the amount of seconds on this state

namespace Brokkr
{
    class ScriptSystemManager;
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
        virtual void ResetStateValues() = 0;
        virtual void Update() = 0;

        void Draw() const
        {
            // TODO: Research Hardware instancing & software instancing
            //if (m_pRenderPipeline)
            //    m_pRenderPipeline->ProcessRenderQueue(); // Draws window
            //else
            //{
            //    m_stateLog.Log(Logger::LogLevel::kError, "RenderPipeline not loaded...");
            //}
        }

        virtual void Destroy() = 0;

        /////////////////////////////////////////////////
        // optional state methods
        /////////////////////////////////////////////////
        virtual void Pause() {}
        virtual void Resume() {}

    protected:

        // System handles for easy access to things that are commonly used
        CoreSystems* m_pCoreSystems = nullptr;

    };
}