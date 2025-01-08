#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <algorithm>

namespace Core { void PrintHelloWorld(); }

namespace Brokkr
{
    class CoreSystems;

    class System
    {

    public:
        explicit System(CoreSystems* pCoreManager)
            : m_pCoreManager(pCoreManager)
        {
            //
        }

        virtual ~System() = default;
        virtual void Destroy() = 0;

    protected:
        CoreSystems* m_pCoreManager;
    };

    class CoreSystems
    {

        int frameCount = 0;
        double frameTime = 0;

    protected:
        std::vector<std::unique_ptr<System>> m_pCoreSubsystems;
        bool isRunning = true;
        std::chrono::time_point<std::chrono::steady_clock> lastFrameTime = std::chrono::high_resolution_clock::now();

        double m_DeltaTime = 0;
        int m_currentFPS = 0;

    public:
        virtual ~CoreSystems() = default;

        void UpdateDelta();
        [[nodiscard]] double GetLastDeltaTime() const { return m_DeltaTime; }

        template <typename CoreSubsystem>
        CoreSubsystem* GetCoreSystem()
        {
            // Iterate through all components in the vector
            for (auto& subsystem : m_pCoreSubsystems)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(subsystem.get()))
                {
                    return target;
                }
            }
            // const std::string error = "Error Core System Failed to : Get a Subsystem!";
            return nullptr; // If no system of type is found
        }

        template <typename CoreSubsystem, typename ... Args>
        CoreSubsystem* AddCoreSystem(Args&&... args)
        {
            static_assert(std::is_base_of_v<System, CoreSubsystem>,
                "System must derive from Brokkr::System");

            std::unique_ptr<CoreSubsystem> newCoreSubsystem = std::make_unique<CoreSubsystem>(this, std::forward<Args>(args)...);
            CoreSubsystem* result = newCoreSubsystem.get(); // Get a raw pointer
            m_pCoreSubsystems.emplace_back(std::move(newCoreSubsystem));
            return result; // Return a pointer
        }

        template<typename CoreSubsystem>
        void RemoveCoreSystem(CoreSubsystem* System)
        {
            // Iterate through all components in the vector
            for (size_t i = 0; i < m_pCoreSubsystems.size(); ++i)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(m_pCoreSubsystems[i].get()))
                {
                    if (target == System) // If the found type
                    {
                        target->Destroy();

                        // Swap and pop the element at index i
                        std::swap(m_pCoreSubsystems[i], m_pCoreSubsystems.back());
                        m_pCoreSubsystems.pop_back();
                        break; // Stop searching
                    }
                }
            }
        }

        // Check if a subsystem exists
        template <typename CoreSubsystem>
        bool IsSystemAvailable()
        {
            return std::any_of(m_pCoreSubsystems.begin(), m_pCoreSubsystems.end(),
                [](const std::unique_ptr<System>& system) 
                {
                    return typeid(*system) == typeid(CoreSubsystem);
                });
        }

        virtual void Destroy() = 0;
    };
}