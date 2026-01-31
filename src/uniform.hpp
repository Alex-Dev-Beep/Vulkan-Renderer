#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct depthResources {
    public:
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
};

struct uniformBuffers {
    public:
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
};

struct buffers {
    public:
    std::vector<VkCommandBuffer> commandBuffers;
};

void createDescriptorSetLayout();

void createUniformBuffers();
void updateUniformBuffer(
    uint32_t currentImage, 
    std::vector<void*> uniformBuffersMapped, 
    VkExtent2D swapChainExtent
);
void createDescriptorPool();

void createDescriptorSets();

void createDepthResources();

VkFormat findSupportedFormat(
    const std::vector<VkFormat>& candidates, 
    VkImageTiling tiling, 
    VkFormatFeatureFlags features,
    VkPhysicalDevice physicalDevice
);

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

bool hasStencilComponent(VkFormat format);

extern depthResources DepthResources;
extern uniformBuffers UniformBuffers;
extern buffers Buffers;