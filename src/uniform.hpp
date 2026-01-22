#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

void createDescriptorSetLayout(
    VkDevice device, 
    VkDescriptorSetLayout& descriptorSetLayout
);
void createUniformBuffers(
    int MAX_FRAMES_IN_FLIGHT, 
    VkDevice device, 
    std::vector<VkBuffer>& uniformBuffers,
    std::vector<VkDeviceMemory>& uniformBuffersMemory,
    std::vector<void*>& uniformBuffersMapped,
    VkPhysicalDevice physicalDevice
);
void updateUniformBuffer(
    uint32_t currentImage, 
    std::vector<void*> uniformBuffersMapped, 
    VkExtent2D swapChainExtent
);
void createDescriptorPool(
    int MAX_FRAMES_IN_FLIGHT, 
    VkDescriptorPool& descriptorPool, 
    VkDevice device
);

void createDescriptorSets(
    uint32_t MAX_FRAMES_IN_FLIGHT,
    VkDescriptorPool descriptorPool,
    VkDescriptorSetLayout descriptorSetLayout,
    std::vector<VkDescriptorSet>& descriptorSets,
    VkDevice device,
    const std::vector<VkBuffer>& uniformBuffers,
    VkSampler textureSampler,
    VkImageView textureImageView
);