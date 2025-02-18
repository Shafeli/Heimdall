#pragma once

#include "VulkanShaderLibrary.hpp"
#include "VulkanWrapper.hpp"
#include "2DRendering/SDLRenderer.h"

class VulkanRenderer final : public Brokkr::SDLRenderer
{
    GAP311::VulkanWrapper m_vulkan;
    GAP311::VulkanShaderLibrary m_vkShaderLibrary;
    GAP311::VulkanWrapper::ConfigOptions m_vulkanConfigOptions;
    GAP311::VulkanShaderLibrary::ConfigOptions m_vulkanShaderConfigOptions;
    GAP311::GraphicsPipelineConfig m_graphicsPipelineConfig;
    std::shared_ptr<GAP311::VulkanWrapper::GraphicsPipeline> m_pGraphicsPipeline = nullptr;

    Brokkr::SDLWindow* m_pWindow = nullptr;

public:

    VulkanRenderer(Brokkr::SDLWindow* pWindow)
        : SDLRenderer(pWindow)
        , m_pWindow(pWindow)
    {
        BuildRenderer();
    }

    virtual ~VulkanRenderer() override;

    virtual void BuildRenderer() override;
    virtual void ClearRenderer() override;

    virtual void DisplayRender() override;
    virtual void Destroy() override;

};

