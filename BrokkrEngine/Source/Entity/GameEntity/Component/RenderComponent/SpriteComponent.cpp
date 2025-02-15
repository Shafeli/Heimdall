#include "SpriteComponent.h"

#include <tinyxml2.h>

#include "../../GameEntity.h"
#include "AssetManager/2DTextureManager/TextureManager2D.h"
#include "TransformComponent.h"
#include "AssetManager/2DTextureManager/2DTexture/Texture2D.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

#define DEBUG_LOGGING 0

Brokkr::SpriteComponent::SpriteComponent
(
    GameEntity* pOwner,
    CoreSystems* systemRef,
    const std::string& textureName,
    bool center
)
    : m_pOwner(pOwner)
    , m_textureName(textureName)
    , m_pCoreSystems(systemRef)
    , m_centerTransform(center)
{

}

bool Brokkr::SpriteComponent::Init()
{
    const auto pAssetManager = m_pCoreSystems->GetCoreSystem<AssetManager>();

    m_pTextureManager = pAssetManager->GetAssetSystem<TextureManager2D>();

    m_texture = m_pTextureManager->GetTexture(m_textureName);

    return true; // defaulting this for now not sure there is a need for this one to Init anything
}

void Brokkr::SpriteComponent::Update()
{

}

void Brokkr::SpriteComponent::Destroy()
{
    // Not sure there is anything to Destroy in this one 
}

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

void Brokkr::SpriteComponent::RegisterCreationFunction(EntityXMLParser* parser)
{
    parser->RegisterComponentCreationFunction("RenderComponent", CreateComponent);
}

void Brokkr::SpriteComponent::CreateComponent(GameEntity* entity, tinyxml2::XMLElement* element,
    CoreSystems* coreSystems)
{
    const char* texturePath = element->Attribute("texture");
    bool center = element->BoolAttribute("center");

    // create SpriteComponent using texture name
    entity->AddComponent<SpriteComponent>(coreSystems, texturePath, center);
}

