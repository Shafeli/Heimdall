#include "VulkanRenderer.h"

#include <filesystem>

#include "VulkanWrapper.hpp"
#include "2DRendering/SDLWindow.h"

VulkanRenderer::~VulkanRenderer()
{
    Destroy();
}

void VulkanRenderer::BuildRenderer()
{
    // Initialize Vulkan with SDL window surface creation
    GAP311::VulkanWrapper::CreateSurfaceFunc createSurfaceFunc = GAP311::VulkanWrapper::CreateVulkanSurfaceSDL(m_pWindow->GetWindow());

    m_vulkanConfigOptions.configureInstanceFunc = [](vkb::InstanceBuilder& builder) -> bool
    {
        builder.require_api_version(1, 3);
        return true;
    };

    m_vulkanConfigOptions.debugLogFunc = GAP311::VulkanWrapper::VulkanLogToSTDOUT();

    if (!m_vulkan.Initialize("VulkanWindow", GAP311::VulkanWrapper::CreateVulkanSurfaceSDL(m_pWindow->GetWindow()), m_vulkanConfigOptions))
    {
        throw std::runtime_error("Failed to initialize Vulkan!");
    }

    m_vulkanShaderConfigOptions.logMessage = [](const char* msg)
    {
        SDL_Log("ShaderLibrary: %s", msg);
    };

    m_vulkanShaderConfigOptions.searchDirs = 
    {
    std::filesystem::current_path().string(), // working dir
    std::filesystem::path().parent_path().string(), // exe dir
    std::filesystem::relative(std::filesystem::path(__FILE__).parent_path()).string(), // source file dir
    };

    if (!m_vkShaderLibrary.Initialize(m_vulkan.GetDevice(), m_vulkanShaderConfigOptions))
    {
        SDL_Log("Failed initializing VulkanShaderLibrary.");
        m_vulkan.Shutdown();
    }

    m_graphicsPipelineConfig.shaderStages.push_back
    (
        vk::PipelineShaderStageCreateInfo()
        .setStage(vk::ShaderStageFlagBits::eVertex)
        .setPName("main")
        .setModule(m_vkShaderLibrary.GetModule("triangle_hardcoded.vert"))
    );
    m_graphicsPipelineConfig.shaderStages.push_back
    (
        vk::PipelineShaderStageCreateInfo()
        .setStage(vk::ShaderStageFlagBits::eFragment)
        .setPName("main")
        .setModule(m_vkShaderLibrary.GetModule("vertex_color.frag"))
    );

    m_pGraphicsPipeline = m_vulkan.CreatePipeline(m_graphicsPipelineConfig);
}

void VulkanRenderer::ClearRenderer()
{
    // Begin a new frame
    vk::CommandBuffer commandBuffer;
    vk::Framebuffer framebuffer;

    if (!m_vulkan.BeginFrame(commandBuffer, framebuffer))
    {
        throw std::runtime_error("Failed to begin Vulkan frame!");
    }

    // Clear the screen to a color (black)
    vk::ClearValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    vk::RenderPassBeginInfo renderPassInfo
    (
        m_vulkan.GetDisplayRenderPass(),
        framebuffer,
        vk::Rect2D({ 0, 0 }, m_vulkan.GetSwapchain().extent),
        1, &clearColor
    );

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    commandBuffer.endRenderPass();
}

void VulkanRenderer::DisplayRender()
{
    {
        vk::CommandBuffer commands;
        vk::Framebuffer framebuffer;
        if (!m_vulkan.BeginFrame(commands, framebuffer))
            return;

        vk::ClearValue clearValues[] =
        {
            vk::ClearColorValue(0.05f, 0.05f, 0.05f, 0.0f),
        };

        vk::RenderPassBeginInfo rpbi{};
        rpbi.setRenderPass(m_vulkan.GetDisplayRenderPass());
        rpbi.setFramebuffer(framebuffer);
        rpbi.setClearValues(clearValues);
        rpbi.setRenderArea(vk::Rect2D({ 0, 0 }, { static_cast<uint32_t>(m_pWindow->GetWindowWidth()), static_cast<uint32_t>(m_pWindow->GetWindowHeight()) }));
        commands.beginRenderPass(rpbi, vk::SubpassContents::eInline);

#if EXAMPLE_USE_VERTEX_BUFFER
        commands.bindVertexBuffers(0, { positionsBuffer, colorsBuffer }, { 0, 0 });
#endif

        commands.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pGraphicsPipeline->GetPipeline());
        commands.draw(3, 1, 0, 0);

        commands.endRenderPass();
        m_vulkan.EndFrame();
    }
}


void VulkanRenderer::Destroy()
{
    m_vulkan.Shutdown();

}
