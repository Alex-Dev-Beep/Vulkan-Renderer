#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription binding{};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attrs{};

        attrs[0].binding = 0;
        attrs[0].location = 0;
        attrs[0].format = VK_FORMAT_R32G32_SFLOAT;
        attrs[0].offset = offsetof(Vertex, pos);

        attrs[1].binding = 0;
        attrs[1].location = 1;
        attrs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attrs[1].offset = offsetof(Vertex, color);

        return attrs;
    }
};


void createVertexBuffer(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    VkCommandPool commandPool,
    VkQueue graphicsQueue,
    VkBuffer& vertexBuffer,
    VkDeviceMemory& vertexBufferMemory,
    const std::vector<Vertex>& vertices
);

void createBuffer(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory
);

uint32_t findMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties
);