#pragma once

//These are needed for components to create them self from XML 
namespace tinyxml2
{
    class XMLElement;
}

namespace Brokkr
{
    class Component
    {
    protected:
        // inline static Logger m_fileLog{ "ComponentLog" };
    public:
        virtual ~Component() = default;

        virtual bool Init() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;  // : This method could be called when the component is destroyed or removed from memory.
        virtual void Render() {}
        virtual void Attach() {}   // : This method could be called when the component is attached to an entity or added to the game world.
        virtual void Detach() {}   // : This method could be called when the component is detached from an entity or removed from the game world.
        virtual void Enable() {}   // : This method could be called when the component is enabled.
        virtual void Disable() {}  // : This method could be called when the component is disabled.
        virtual void LateUpdate() {}
    };
}


