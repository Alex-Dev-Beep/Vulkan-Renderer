#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vertex.hpp"

#include <optional>
#include <vector>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct device {
    public:
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkCommandPool commandPool;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

struct queues {
    public:
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct semaphores {
    public:
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
};

struct fences {
    public:
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

void pickPhysicalDevice();
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
void createLogicalDevice();
void createCommandPool();
void createCommandBuffer();
void recordCommandBuffer();

extern device Device;
extern queues Queues;
extern fences Fences;
extern semaphores Semaphores;