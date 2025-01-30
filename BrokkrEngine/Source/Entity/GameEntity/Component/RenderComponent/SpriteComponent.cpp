#include "SpriteComponent.h"

#include <tinyxml2.h>

#include "../../GameEntity.h"
#include "../../../AssetManager/TextureManager/TextureManager.h"
#include "../TransformComponent/TransformComponent.h"
#include "AssetManager/TextureManager/Texture/Texture.h"
#include "AssetManager/XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

#define DEBUG_LOGGING 0

namespace Brokkr
{
    class TextureManager;
    class ColorComponent;
}

// TODO: Refactor this into a render component that handles static sprites, animations, and basic shapes
Brokkr::SpriteComponent::SpriteComponent
(
    GameEntity* pOwner,
    const std::string& textureName,
    CoreSystems* systemRef,
    float depth,
    bool active,
    bool center,
    const std::string& textureBlendColor
)
    : m_pOwner(pOwner)
    , m_systemRef(systemRef)
    , m_textureName(textureName)
    , m_depth(depth)
    , m_active(active)
    , m_centerTransform(center)
    , m_textureBlendColor(textureBlendColor)
{

}

bool Brokkr::SpriteComponent::Init()
{

    m_pAssetManager = m_systemRef->GetCoreSystem<AssetManager>();
    m_pEventManager = m_systemRef->GetCoreSystem<EventManager>();

    m_texture = m_pAssetManager->GetAssetSystem<TextureManager>()->GetTexture(m_textureName);

    if (m_textureBlendColor.empty())
        return true;

    SetColor(m_textureBlendColor);

    return true; // defaulting this for now not sure there is a need for this one to Init anything
}

void Brokkr::SpriteComponent::Update()
{

}

void Brokkr::SpriteComponent::Destroy()
{
    // Not sure there is anything to Destroy in this one 
}

// TODO: split this into a render component and a sprite or something 
void Brokkr::SpriteComponent::Render()
{
     // TODO: Renderer : render redo after renderer and render sorting is down again 
}

void Brokkr::SpriteComponent::Attach()
{
    //
}

void Brokkr::SpriteComponent::Detach()
{
    //
}

void Brokkr::SpriteComponent::Enable()
{
    Component::Enable();
    m_active = true;
}

void Brokkr::SpriteComponent::Disable()
{
    Component::Disable();
    m_active = false;
}

void Brokkr::SpriteComponent::SetBlendColor()
{
    if (m_textureBlendColor.empty())
        return;

    SetColor(m_textureBlendColor);
}

void Brokkr::SpriteComponent::SetColor(const std::string& color)
{
    if (m_texture)
    {
        m_texture->SetTextureColor({ color });
    }
}

void Brokkr::SpriteComponent::ResetColor()
{
    if (m_texture)
        m_texture->ResetTextureColor();
}

void Brokkr::SpriteComponent::RegisterCreationFunction(EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("RenderComponent", CreateComponent);
}

void Brokkr::SpriteComponent::CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element,
    CoreSystems* coreSystems)
{
    const char* texturePath = element->Attribute("texture");
    const char* TextColorStr = element->Attribute("blendColor");
    const float renderDepth = element->FloatAttribute("depth");
    bool active = element->BoolAttribute("active");
    bool center = element->BoolAttribute("center");

    if (TextColorStr == nullptr)
    {
        entity->AddComponent<SpriteComponent>(texturePath, coreSystems, renderDepth, active, center, "");
        return;
    }
    // create SpriteComponent using texture name
    entity->AddComponent<SpriteComponent>(texturePath, coreSystems, renderDepth, active, center, TextColorStr);
}

