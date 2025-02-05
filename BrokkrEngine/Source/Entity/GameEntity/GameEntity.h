#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Component/Component.h"
#include "Utility/IDGenerator.h"


namespace Brokkr
{

    class GameEntity
    {
        int m_id;
        bool isEnabled = true;
        std::vector<std::unique_ptr<Component>> m_pComponents;
    public:

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        GameEntity() : m_id(IDGenerator::GenerateUniqueID()){}

        GameEntity(const GameEntity& other) = delete;
        GameEntity& operator=(const GameEntity& other) = delete;
        GameEntity(GameEntity&& other) noexcept = default;
        GameEntity& operator=(GameEntity&& other) noexcept = default;

        ~GameEntity();

        void Init() const;

        [[nodiscard]] int GetId() const { return m_id; }

        // Object Update Components
        ///////////////////////////////////////////
        void Update() const;

        // Object Rendering / Maybe keep will see want this gone as well
        ///////////////////////////////////////////
        void Render() const;

        // Object Disable
        ///////////////////////////////////////////
        void Disable();

        // Object Enable
        ///////////////////////////////////////////
        void Enable();

        // Object Serialize TODO: Flesh out this system more
        ///////////////////////////////////////////
        void Serialize(const std::string& filePath) const;

        // Object Deserialize TODO: Flesh out this system more
        ///////////////////////////////////////////
        void Deserialize(const std::string& filePath) const;

        // Object Get a Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        ComponentType* GetComponent();

        // Object Remove Component of type
        ///////////////////////////////////////////
        template<typename ComponentType>
        void RemoveComponent()
        {
            // Iterate through all components in the vector
            for (auto it = m_pComponents.begin(); it != m_pComponents.end();)
            {
                ComponentType* target = dynamic_cast<ComponentType*>(it->get());

                // If the cast is successful
                if (target)
                {
                    target->Detach();
                    target->Destroy();

                    std::swap(*it, m_pComponents.back());
                    m_pComponents.pop_back();
                    return;
                }
                ++it;
            }


        }

        // Object Get all Components of type
        ///////////////////////////////////////////
        template<typename ComponentType>
        std::vector<ComponentType*> GetAllComponents()
        {
        
            std::vector<ComponentType*> components;

            // Iterate through all components in the vector
            for (auto& component : m_pComponents)
            {
                // Attempt to dynamically cast the component to the specified type
                ComponentType* target = dynamic_cast<ComponentType*>(component.get());

                // If the cast is successful
                if (target)
                {
                    components.push_back(target);
                }
            }

            // If no component of type is found
            return components;
        }

        // Object Attach Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallAttachOnComponent();

        // Object Detach Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallDetachOnComponent();
         
        // Object Enable Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallEnableOnComponent();

        // Object Disable Call on Component 
        ///////////////////////////////////////////
        template<typename ComponentType>
        void CallDisableOnComponent();
        /*template <class ComponentType, class ... Args>
        ComponentType* AddComponent(const std::string& xmlString, Args&& ... args);*/

        // Object Add a Component
        ///////////////////////////////////////////
        template<typename ComponentType, typename... Args>
        ComponentType* AddComponent(Args&&... args);

        //TODO: new idea is to take in a str name of component to type check like lua as well as a vector of 
        // strings that are the XML attributes of so the component can construct it self and the XML will
        // still hold to SoC but still kind of break it but making the component responsible for
        // their behavior as well as their construction.
        //
        // another idea is to just build a long string in xml parsing and just send the string to the component
        // and the component and string parse for the data it need to construct might be faster then string and vector
        //
        // another idea pack the data into a buffer and reinterpret it as a struct of the data needed to build
        // the component
        
        /*template <typename ComponentType, typename ... Args>
        ComponentType* AddComponentFromString(const std::string& str, Args&&... args)
        {
            // Convert the string to the desired component type
            ComponentType component = ComponentType::FromString(str);

            // Create a instance of the component type passing in the current GameEntity pointer and the component
            std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, component, std::forward<Args>(args)...);

            // Get a raw pointer to the component
            ComponentType* result = newComponent.get();

            // Add the component to the vector
            m_pComponents.emplace_back(std::move(newComponent));

            // Return a pointer
            return result;
        }*/
    };

    template <typename ComponentType>
    ComponentType* GameEntity::GetComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                return target;
            }
        }

        // If no component of type is found
        return nullptr;
    }

    template <typename ComponentType>
    void GameEntity::CallAttachOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Attach();
            }
        }
    }

    template <typename ComponentType>
    void GameEntity::CallDetachOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Detach();
            }
        }
    }

    template<typename ComponentType>
    inline void GameEntity::CallEnableOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Enable();
            }
        }
    }

    template<typename ComponentType>
    inline void GameEntity::CallDisableOnComponent()
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                target->Disable();
            }
        }
    }

    /*template <typename ComponentType, typename... Args>
    inline ComponentType* GameEntity::AddComponent(const std::string& xmlString, Args&&... args)
    {
        // Create an instance of the component type passing the current GameEntity pointer
        std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);

        // Parse the XML string to init the component
        newComponent->ParseFromXML(xmlString);

        // Get a raw pointer to the component
        ComponentType* result = newComponent.get();

        // Add the component to the vector
        m_pComponents.emplace_back(std::move(newComponent));

        // Return a pointer
        return result;
    }*/

    template <typename ComponentType, typename ... Args>
    ComponentType* GameEntity::AddComponent(Args&&... args)
    {
        // Create a instance of the component type passing in the current GameEntity pointer
        std::unique_ptr<ComponentType> newComponent = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);

        // Get a raw pointer to the component
        ComponentType* result = newComponent.get();

        // Add the component to the vector
        m_pComponents.emplace_back(std::move(newComponent));

        // Return a pointer
        return result;
    }
}
