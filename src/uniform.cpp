#include "uniform.hpp"
#include "vertex.hpp"
#include "image.hpp"
#include "device.hpp"
#include "pipeline.hpp"
#include "swapchain.hpp"
#include "renderer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

depthResources DepthResources;
uniformBuffers UniformBuffers;
buffers Buffers;

void createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    

    if (vkCreateDescriptorSetLayout(Device.device, &layoutInfo, nullptr, &Pipeline.descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout!");
    } else {
        std::cout << "Succesfully created descriptor set layout!" << std::endl;
    }
}

void createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    UniformBuffers.uniformBuffers.resize(Renderer.MAX_FRAMES_IN_FLIGHT);
    UniformBuffers.uniformBuffersMemory.resize(Renderer.MAX_FRAMES_IN_FLIGHT);
    UniformBuffers.uniformBuffersMapped.resize(Renderer.MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        // createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        createBuffer(Device.physicalDevice, Device.device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers.uniformBuffers[i], UniformBuffers.uniformBuffersMemory[i]);

        vkMapMemory(Device.device, UniformBuffers.uniformBuffersMemory[i], 0, bufferSize, 0, &UniformBuffers.uniformBuffersMapped[i]);
    }
}

void updateUniformBuffer(uint32_t currentImage, 
    std::vector<void*> uniformBuffersMapped, 
    VkExtent2D swapChainExtent
) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(Renderer.MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(Renderer.MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(Renderer.MAX_FRAMES_IN_FLIGHT);

    poolInfo.maxSets = static_cast<uint32_t>(Renderer.MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(Device.device, &poolInfo, nullptr, &Device.descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }

    std::cout << "Succesfully created descriptor pool!" << std::endl;
}

void createDescriptorSets() {

    std::vector<VkDescriptorSetLayout> layouts(Renderer.MAX_FRAMES_IN_FLIGHT, Pipeline.descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = Device.descriptorPool;
    allocInfo.descriptorSetCount = Renderer.MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    Device.descriptorSets.resize(Renderer.MAX_FRAMES_IN_FLIGHT);

    if (vkAllocateDescriptorSets(Device.device, &allocInfo, Device.descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    std::cout << "Successfully allocated descriptor sets!" << std::endl;

    std::vector<VkDescriptorBufferInfo> bufferInfos(Renderer.MAX_FRAMES_IN_FLIGHT);
    std::vector<VkWriteDescriptorSet> descriptorWrites(Renderer.MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = UniformBuffers.uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = Image.textureImageView;
        imageInfo.sampler = Image.textureSampler;

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = Device.descriptorSets[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = Device.descriptorSets[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(Device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}
}

void createDepthResources() {
    VkFormat depthFormat = findDepthFormat(Device.physicalDevice);

    createImage(SwapChain.swapChainExtent.width, SwapChain.swapChainExtent.height, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, DepthResources.depthImage, DepthResources.depthImageMemory, Device.device, Device.physicalDevice);
    DepthResources.depthImageView = createImageView(DepthResources.depthImage, depthFormat, Device.device, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    transitionImageLayout(DepthResources.depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, Device.device, Device.commandPool, Queues.graphicsQueue, 1);
}

VkFormat findSupportedFormat(
    const std::vector<VkFormat>& candidates, 
    VkImageTiling tiling, 
    VkFormatFeatureFlags features,
    VkPhysicalDevice physicalDevice
) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format!");
}

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
        physicalDevice
    );
}

bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}