#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "BrokkrDefaultScene.h"
#include "Scene.h"

namespace Brokkr
{
    // TODO: SceneManager
    // * Redo adding Scenes to temple-ted system
    class SceneManager final : public System
    {
        EventManager* m_pEventManager = nullptr;

        std::unordered_map<std::string, std::unique_ptr<Scene>> m_states;
        std::string m_pActiveStateKey;
        std::string m_pendingStateKey;

        Scene* m_pActiveState = nullptr;
        bool m_isReplacing = false;

    public:
        explicit SceneManager(CoreSystems* pCoreManager)
            : System(pCoreManager)
        {
            //
            m_pEventManager = pCoreManager->GetCoreSystem<EventManager>();

            AddState("BrokkrEngineDefault", std::make_unique<BrokkrDefaultScene>(pCoreManager));
            SetActiveState("BrokkrEngineDefault");
        }

        virtual ~SceneManager() override;
        void AddState(const std::string& stateIdentifier, std::unique_ptr<Scene> newState);
        void RemoveState(const std::string& stateIdentifier);
        void SetActiveState(const std::string& stateIdentifier);
        void ResetState(const std::string& stateIdentifier);

        virtual void Destroy() override;
        [[nodiscard]] Scene* GetActiveState();

    protected: // Only calls these two in core main game loop
        friend class CoreSystems;
        void ProcessStateChange();
        //[[nodiscard]] Scene* GetActiveState();
    };
}

