#pragma once

#include <stb_image.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct image {
    public:
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
};

void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice device, VkPhysicalDevice physicalDevice);
void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue graphicsQueue, VkDevice device, VkCommandPool commandPool);
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkQueue graphicsQueue, VkDevice device, VkCommandPool commandPool);
void createTextureImageView();
void createTextureSampler();
void createTextureImage();

VkImageView createImageView(VkImage image, VkFormat format, VkDevice device, VkImageAspectFlags aspectFlags);
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);

extern image Image;