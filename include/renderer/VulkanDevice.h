#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace fresh {

/**
 * @brief Manages Vulkan physical and logical device
 * 
 * Handles device selection, queue family detection, and logical device creation.
 */
class VulkanDevice {
public:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        
        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanDevice();

    // Prevent copying
    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;

    /**
     * @brief Initialize the device
     * @return true if initialization was successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Get the physical device
     * @return Vulkan physical device handle
     */
    VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }

    /**
     * @brief Get the logical device
     * @return Vulkan logical device handle
     */
    VkDevice getDevice() const { return m_device; }

    /**
     * @brief Get the graphics queue
     * @return Vulkan graphics queue handle
     */
    VkQueue getGraphicsQueue() const { return m_graphicsQueue; }

    /**
     * @brief Get the present queue
     * @return Vulkan present queue handle
     */
    VkQueue getPresentQueue() const { return m_presentQueue; }

    /**
     * @brief Get the queue family indices
     * @return QueueFamilyIndices structure
     */
    const QueueFamilyIndices& getQueueFamilyIndices() const { return m_queueFamilyIndices; }

    /**
     * @brief Cleanup device resources
     */
    void cleanup();

private:
    bool selectPhysicalDevice();
    bool createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);

private:
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    QueueFamilyIndices m_queueFamilyIndices;
};

} // namespace fresh
