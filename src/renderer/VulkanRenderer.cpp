#include "renderer/VulkanRenderer.h"
#include "renderer/VulkanDevice.h"
#include "renderer/VulkanPipeline.h"
#include "renderer/ShaderManager.h"
#include "core/Window.h"
#include <iostream>
#include <vector>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace fresh {

VulkanRenderer::VulkanRenderer(Window* window)
    : m_window(window)
    , m_instance(VK_NULL_HANDLE)
    , m_surface(VK_NULL_HANDLE)
    , m_swapchain(VK_NULL_HANDLE)
    , m_renderPass(VK_NULL_HANDLE)
    , m_commandPool(VK_NULL_HANDLE)
    , m_currentFrame(0)
    , m_imageIndex(0)
{
}

VulkanRenderer::~VulkanRenderer() {
    cleanup();
}

bool VulkanRenderer::initialize() {
    std::cout << "Initializing Vulkan renderer..." << std::endl;
    
    if (!createInstance()) {
        return false;
    }
    std::cout << "Vulkan instance created" << std::endl;
    
    if (!createSurface()) {
        return false;
    }
    std::cout << "Vulkan surface created" << std::endl;
    
    m_device = std::make_unique<VulkanDevice>(m_instance, m_surface);
    if (!m_device->initialize()) {
        return false;
    }
    std::cout << "Vulkan device created" << std::endl;
    
    if (!createSwapchain()) {
        return false;
    }
    std::cout << "Swapchain created" << std::endl;
    
    if (!createRenderPass()) {
        return false;
    }
    std::cout << "Render pass created" << std::endl;
    
    if (!createFramebuffers()) {
        return false;
    }
    std::cout << "Framebuffers created" << std::endl;
    
    if (!createCommandPool()) {
        return false;
    }
    std::cout << "Command pool created" << std::endl;
    
    if (!createCommandBuffers()) {
        return false;
    }
    std::cout << "Command buffers created" << std::endl;
    
    if (!createSyncObjects()) {
        return false;
    }
    std::cout << "Sync objects created" << std::endl;
    
    m_shaderManager = std::make_unique<ShaderManager>(m_device.get());
    m_pipeline = std::make_unique<VulkanPipeline>(m_device.get(), m_renderPass);
    
    return true;
}

bool VulkanRenderer::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Fresh Voxel Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Fresh Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;
    
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance" << std::endl;
        return false;
    }
    
    return true;
}

bool VulkanRenderer::createSurface() {
    if (glfwCreateWindowSurface(m_instance, m_window->getHandle(), nullptr, &m_surface) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface" << std::endl;
        return false;
    }
    return true;
}

bool VulkanRenderer::createSwapchain() {
    // Simplified swapchain creation
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->getPhysicalDevice(), m_surface, &capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_device->getPhysicalDevice(), m_surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_device->getPhysicalDevice(), m_surface, &formatCount, formats.data());
    
    VkSurfaceFormatKHR surfaceFormat = formats[0];
    
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;
    createInfo.minImageCount = capabilities.minImageCount + 1;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = capabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;
    
    if (vkCreateSwapchainKHR(m_device->getDevice(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
        std::cerr << "Failed to create swapchain" << std::endl;
        return false;
    }
    
    // Get swapchain images
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapchain, &imageCount, nullptr);
    m_swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapchain, &imageCount, m_swapchainImages.data());
    
    // Create image views
    m_swapchainImageViews.resize(m_swapchainImages.size());
    for (size_t i = 0; i < m_swapchainImages.size(); i++) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_swapchainImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = surfaceFormat.format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(m_device->getDevice(), &viewInfo, nullptr, &m_swapchainImageViews[i]) != VK_SUCCESS) {
            std::cerr << "Failed to create image view" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool VulkanRenderer::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB; // Simplified
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    
    if (vkCreateRenderPass(m_device->getDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        std::cerr << "Failed to create render pass" << std::endl;
        return false;
    }
    
    return true;
}

bool VulkanRenderer::createFramebuffers() {
    m_framebuffers.resize(m_swapchainImageViews.size());
    
    for (size_t i = 0; i < m_swapchainImageViews.size(); i++) {
        VkImageView attachments[] = { m_swapchainImageViews[i] };
        
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_window->getWidth();
        framebufferInfo.height = m_window->getHeight();
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(m_device->getDevice(), &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
            std::cerr << "Failed to create framebuffer" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool VulkanRenderer::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_device->getQueueFamilyIndices().graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    if (vkCreateCommandPool(m_device->getDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        std::cerr << "Failed to create command pool" << std::endl;
        return false;
    }
    
    return true;
}

bool VulkanRenderer::createCommandBuffers() {
    m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
    
    if (vkAllocateCommandBuffers(m_device->getDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        std::cerr << "Failed to allocate command buffers" << std::endl;
        return false;
    }
    
    return true;
}

bool VulkanRenderer::createSyncObjects() {
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(m_device->getDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_device->getDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device->getDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            std::cerr << "Failed to create sync objects" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool VulkanRenderer::beginFrame() {
    vkWaitForFences(m_device->getDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    
    VkResult result = vkAcquireNextImageKHR(m_device->getDevice(), m_swapchain, UINT64_MAX,
                                           m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        return false;
    }
    
    vkResetFences(m_device->getDevice(), 1, &m_inFlightFences[m_currentFrame]);
    
    vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if (vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo) != VK_SUCCESS) {
        std::cerr << "Failed to begin recording command buffer" << std::endl;
        return false;
    }
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_framebuffers[m_imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {m_window->getWidth(), m_window->getHeight()};
    
    VkClearValue clearColor = {{{0.1f, 0.2f, 0.3f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    return true;
}

void VulkanRenderer::endFrame() {
    vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);
    
    if (vkEndCommandBuffer(m_commandBuffers[m_currentFrame]) != VK_SUCCESS) {
        std::cerr << "Failed to record command buffer" << std::endl;
        return;
    }
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];
    
    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    if (vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
        std::cerr << "Failed to submit draw command buffer" << std::endl;
        return;
    }
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = {m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &m_imageIndex;
    
    vkQueuePresentKHR(m_device->getPresentQueue(), &presentInfo);
    
    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::waitIdle() {
    if (m_device) {
        vkDeviceWaitIdle(m_device->getDevice());
    }
}

void VulkanRenderer::cleanup() {
    if (m_device) {
        vkDeviceWaitIdle(m_device->getDevice());
        
        for (auto semaphore : m_imageAvailableSemaphores) {
            vkDestroySemaphore(m_device->getDevice(), semaphore, nullptr);
        }
        for (auto semaphore : m_renderFinishedSemaphores) {
            vkDestroySemaphore(m_device->getDevice(), semaphore, nullptr);
        }
        for (auto fence : m_inFlightFences) {
            vkDestroyFence(m_device->getDevice(), fence, nullptr);
        }
        
        if (m_commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(m_device->getDevice(), m_commandPool, nullptr);
        }
        
        for (auto framebuffer : m_framebuffers) {
            vkDestroyFramebuffer(m_device->getDevice(), framebuffer, nullptr);
        }
        
        if (m_renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(m_device->getDevice(), m_renderPass, nullptr);
        }
        
        for (auto imageView : m_swapchainImageViews) {
            vkDestroyImageView(m_device->getDevice(), imageView, nullptr);
        }
        
        if (m_swapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_device->getDevice(), m_swapchain, nullptr);
        }
        
        m_device->cleanup();
    }
    
    if (m_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }
    
    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
    }
}

} // namespace fresh
