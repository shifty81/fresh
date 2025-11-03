#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <memory>

namespace fresh {

class VulkanDevice;

/**
 * @brief Manages Vulkan graphics pipeline
 * 
 * Creates and manages the graphics pipeline, including shader stages,
 * vertex input, rasterization, and other pipeline states.
 */
class VulkanPipeline {
public:
    VulkanPipeline(VulkanDevice* device, VkRenderPass renderPass);
    ~VulkanPipeline();

    // Prevent copying
    VulkanPipeline(const VulkanPipeline&) = delete;
    VulkanPipeline& operator=(const VulkanPipeline&) = delete;

    /**
     * @brief Create the graphics pipeline
     * @param vertShaderPath Path to vertex shader
     * @param fragShaderPath Path to fragment shader
     * @return true if creation was successful, false otherwise
     */
    bool create(const std::string& vertShaderPath, const std::string& fragShaderPath);

    /**
     * @brief Bind the pipeline to a command buffer
     * @param commandBuffer Command buffer to bind to
     */
    void bind(VkCommandBuffer commandBuffer);

    /**
     * @brief Get the pipeline handle
     * @return Vulkan pipeline handle
     */
    VkPipeline getPipeline() const { return m_pipeline; }

    /**
     * @brief Get the pipeline layout
     * @return Vulkan pipeline layout handle
     */
    VkPipelineLayout getLayout() const { return m_pipelineLayout; }

    /**
     * @brief Cleanup pipeline resources
     */
    void cleanup();

private:
    VkShaderModule createShaderModule(const std::string& code);

private:
    VulkanDevice* m_device;
    VkRenderPass m_renderPass;
    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
};

} // namespace fresh
