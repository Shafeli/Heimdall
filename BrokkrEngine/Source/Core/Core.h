#pragma once
#include <memory>
#include <vector>


namespace Core {

    void InitializeLua();

    void PrintHelloWorld();

}

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

    protected:
        // inline static Logger m_fileLog{ "CoreSystemLog" };
        std::vector<std::unique_ptr<System>> m_pCoreSubsystems;

    public:
        virtual ~CoreSystems() = default;

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
            std::unique_ptr<CoreSubsystem> newCoreSubsystem = std::make_unique<CoreSubsystem>(this, std::forward<Args>(args)...);

            CoreSubsystem* result = newCoreSubsystem.get(); // Get a raw pointer to the component
            m_pCoreSubsystems.emplace_back(std::move(newCoreSubsystem)); // Add the system to the vector

            if (!result)
            {
                // const std::string error = "Error Core System Failed to : Construct a Subsystem!";
            }
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

        virtual void Destroy() = 0;
    };
}