#pragma once

#include "RenderContext.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace fresh {

/**
 * @brief Vulkan implementation of render context
 */
class VulkanRenderContext : public IRenderContext {
public:
    VulkanRenderContext();
    ~VulkanRenderContext() override;
    
    bool initialize(Window* window) override;
    void shutdown() override;
    bool beginFrame() override;
    void endFrame() override;
    void waitIdle() override;
    
    void setViewport(int x, int y, int width, int height) override;
    void setScissor(int x, int y, int width, int height) override;
    void clearColor(float r, float g, float b, float a) override;
    void clearDepth(float depth) override;
    
    GraphicsAPI getAPI() const override { return GraphicsAPI::Vulkan; }
    void* getNativeDevice() override { return &device; }
    
    int getSwapchainWidth() const override { return swapchainWidth; }
    int getSwapchainHeight() const override { return swapchainHeight; }
    
    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode, const std::string& fragmentCode) override;
    
private:
    bool createInstance();
    bool createDevice();
    bool createSwapchain();
    bool createRenderPass();
    bool createFramebuffers();
    bool createCommandPool();
    bool createSyncObjects();
    
    void cleanup();
    
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> swapchainFramebuffers;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;
    
    // Synchronization
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    
    int swapchainWidth = 0;
    int swapchainHeight = 0;
    uint32_t currentFrame = 0;
    uint32_t imageIndex = 0;
    
    Window* window = nullptr;
};

} // namespace fresh
