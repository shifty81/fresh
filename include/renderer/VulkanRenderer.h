#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <memory>

namespace fresh {

class Window;
class VulkanDevice;
class VulkanPipeline;
class ShaderManager;

/**
 * @brief Main Vulkan rendering system
 * 
 * Manages the Vulkan instance, surface, swapchain, and rendering pipeline.
 * Implements the core rendering loop and command buffer submission.
 */
class VulkanRenderer {
public:
    explicit VulkanRenderer(Window* window);
    ~VulkanRenderer();

    // Prevent copying
    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;

    /**
     * @brief Initialize the Vulkan renderer
     * @return true if initialization was successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Begin a new frame
     * @return true if frame can be rendered, false otherwise
     */
    bool beginFrame();

    /**
     * @brief End the current frame and present
     */
    void endFrame();

    /**
     * @brief Wait for device to be idle
     */
    void waitIdle();

    /**
     * @brief Cleanup renderer resources
     */
    void cleanup();

    /**
     * @brief Get the Vulkan device
     * @return pointer to VulkanDevice
     */
    VulkanDevice* getDevice() const { return m_device.get(); }

private:
    bool createInstance();
    bool createSurface();
    bool createSwapchain();
    bool createRenderPass();
    bool createFramebuffers();
    bool createCommandPool();
    bool createCommandBuffers();
    bool createSyncObjects();

private:
    Window* m_window;
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    VkRenderPass m_renderPass;
    VkCommandPool m_commandPool;
    
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanPipeline> m_pipeline;
    std::unique_ptr<ShaderManager> m_shaderManager;
    
    std::vector<VkImage> m_swapchainImages;
    std::vector<VkImageView> m_swapchainImageViews;
    std::vector<VkFramebuffer> m_framebuffers;
    std::vector<VkCommandBuffer> m_commandBuffers;
    
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    
    uint32_t m_currentFrame;
    uint32_t m_imageIndex;
    
    const int MAX_FRAMES_IN_FLIGHT = 2;
};

} // namespace fresh
