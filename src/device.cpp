#include "device.hpp"
#include "validation_layers.hpp"
#include "swapchain.hpp"
#include "vertex.hpp"
#include "surface.hpp"
#include "instance.hpp"
#include "uniform.hpp"
#include "renderer.hpp"
#include "pipeline.hpp"
#include "buffer.hpp"

#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <set>

device Device;
queues Queues;
fences Fences;
semaphores Semaphores;

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data()
    );

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        // Graphics queue
        if (queueFamily.queueCount > 0 &&
            queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            i,
            surface,
            &presentSupport
        );

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(Device.physicalDevice, Surface.surface);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;createInfo.enabledExtensionCount = 0;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    if (vkCreateDevice(Device.physicalDevice, &createInfo, nullptr, &Device.device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    } else {
        std::cout << "Succesfully created Logical Device!" << std::endl;
    }

    vkGetDeviceQueue(Device.device, indices.graphicsFamily.value(), 0, &Queues.graphicsQueue);
    vkGetDeviceQueue(Device.device, indices.presentFamily.value(), 0, &Queues.presentQueue);

}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =
            querySwapChainSupport(device, surface);

        swapChainAdequate =
            !swapChainSupport.formats.empty() &&
            !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(Instance.instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(Instance.instance, &deviceCount, devices.data());
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        std::cout << "Checking GPU: "
                << props.deviceName << std::endl;

        if (isDeviceSuitable(device, Surface.surface)) {
            Device.physicalDevice = device;
            std::cout << "Selected GPU!" << std::endl;
            break;
        }
    }


    if (Device.physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(Device.physicalDevice, &props);
    std::cout << "Using GPU: " << props.deviceName << std::endl;

}

void createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(Device.physicalDevice, Surface.surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(Device.device, &poolInfo, nullptr, &Device.commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }

    std::cout << "Succesfully created command pool!" << std::endl;
}

void createCommandBuffer() {
    Buffers.commandBuffers.resize(Renderer.MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Device.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(Buffers.commandBuffers.size());

    if (vkAllocateCommandBuffers(
            Device.device,
            &allocInfo,
            Buffers.commandBuffers.data()
        ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    std::cout << "Succesfully allocated command buffers!" << std::endl;
}

void recordCommandBuffer() {
    VkCommandBuffer cmd = Buffers.commandBuffers[Renderer.imageIndex];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin command buffer");
    }

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = Pipeline.renderPass;
    renderPassInfo.framebuffer = SwapChain.swapChainFramebuffers[Renderer.imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SwapChain.swapChainExtent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline.graphicsPipeline);

    VkBuffer vertexBuffers[] = { VertexBuffer.vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(cmd, IndexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    VkViewport viewport{};
    viewport.width  = (float)SwapChain.swapChainExtent.width;
    viewport.height = (float)SwapChain.swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.extent = SwapChain.swapChainExtent;
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        Pipeline.pipelineLayout,
        0,
        1,
        &Device.descriptorSets[Renderer.imageIndex],
        0,
        nullptr
    );

    vkCmdDrawIndexed(cmd, static_cast<uint32_t>(Renderer.indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(cmd);

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer");
    }
}
