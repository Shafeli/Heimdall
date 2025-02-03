#include "SceneManager.h"

#include <cassert>
#include <string>

Brokkr::SceneManager::~SceneManager()
{
    Destroy();
}

void Brokkr::SceneManager::AddState(const std::string& stateIdentifier, std::unique_ptr<Scene> newState)
{
    newState->Init();
    m_states[stateIdentifier] = std::move(newState);
}

void Brokkr::SceneManager::RemoveState(const std::string& stateIdentifier)
{
    if (!m_states.empty()) // Check if there are any states in the map
    {
        const auto stateIterator = m_states.find(stateIdentifier); // Find the state with the given key

        if (stateIterator != m_states.end()) // Check if the state was found
        {
            if (m_pActiveStateKey == stateIdentifier) // Check if the state being removed is the active state
            {
                // Pause the active state
                m_pActiveState->Pause();

                if (m_states.size() > 1) // Check if there are any other states in the map
                {
                    const auto nextIterator = stateIterator == std::prev(m_states.end()) ? m_states.begin() : std::next(stateIterator);

                    // Set the next state in the map as the new active state
                    m_pActiveStateKey = nextIterator->first;
                    m_pActiveState = nextIterator->second.get();
                    m_pActiveState->Resume();
                }
                else // If there are no other states in the map, set the active state to null
                {
                    m_pActiveStateKey.clear();
                    m_pActiveState = nullptr;
                }
            }

            // Destroy the state and remove it from the map
            stateIterator->second->Destroy();
            m_states.erase(stateIterator);
        }
    }
}

void Brokkr::SceneManager::SetActiveState(const std::string& stateIdentifier)
{

    // State is already active, do nothing
    if (stateIdentifier == m_pActiveStateKey)
    {
        const std::string error = "Active state set to current active state key: " + stateIdentifier;
        assert(false); // assertion fail
        return;
    }

    const auto stateIterator = m_states.find(stateIdentifier);

    if (stateIterator == m_states.end()) 
    {
        const std::string error = "Missing state key: " + stateIdentifier + " is unknown";
        assert(false); // assertion fail
    }

    /*if (m_pActiveState != nullptr)
    {
        // pause current state
        m_pActiveState->Pause();
    }*/

    m_pEventManager->DumpEvents();
    // set new active state key
    m_pendingStateKey = stateIdentifier;

    m_isReplacing = true;
}

void Brokkr::SceneManager::ResetState(const std::string& stateIdentifier)
{
    const auto stateIterator = m_states.find(stateIdentifier);

    if (stateIterator == m_states.end())
    {
        const std::string error = "State key: " + stateIdentifier + " does not exist";
        assert(false); // assertion fail
    }

    if (m_states[stateIdentifier] != nullptr)
    {
        m_states[stateIdentifier]->ResetStateValues();
    }
}

void Brokkr::SceneManager::ProcessStateChange()
{
    if (m_isReplacing)
    {
        auto stateIter = m_states.find(m_pendingStateKey);
        if (stateIter != m_states.end())
        {

            if (m_pActiveState != nullptr)
            {
                // pause current state
                m_pActiveState->Pause();
            }

            // set new active state key
            m_pActiveStateKey = m_pendingStateKey;

            // set new active state
            m_pActiveState = stateIter->second.get();

            m_pActiveState->Resume();

            m_isReplacing = false;
        }
    }
}

Brokkr::Scene* Brokkr::SceneManager::GetActiveState()
{
    if (m_pActiveState == nullptr)
    {
        const std::string error = "Active State Is Null";
    }
    return m_pActiveState;
}

void Brokkr::SceneManager::Destroy()
{
    for (const auto& [key, state] : m_states)
    {
        state->Destroy();
    }
    m_states.clear();
}
