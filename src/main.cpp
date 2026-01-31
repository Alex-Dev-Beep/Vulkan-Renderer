#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>
#include <vector>

#include "window.hpp"
#include "instance.hpp"
#include "surface.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "sync.hpp"
#include "vertex.hpp"
#include "uniform.hpp"
#include "image.hpp"
#include "renderer.hpp"
#include "buffer.hpp"

// doccument everything

bool framebufferResized = false;

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    framebufferResized = true;
}

void drawFrame(
    VkDevice& device,
    VkSwapchainKHR& swapChain,
    VkRenderPass& renderPass,
    std::vector<VkFramebuffer>& swapChainFramebuffers, 
    VkExtent2D& swapChainExtent, 
    VkPipeline& graphicsPipeline, 
    VkQueue& graphicsQueue, 
    VkQueue& presentQueue,
    std::vector<VkFence>& inFlightFences, 
    std::vector<VkSemaphore>& renderFinishedSemaphores, 
    std::vector<VkSemaphore>& imageAvailableSemaphores, 
    std::vector<VkCommandBuffer>& commandBuffers, 
    int MAX_FRAMES_IN_FLIGHT, 
    VkSurfaceKHR surface, 
    GLFWwindow* window, 
    VkPhysicalDevice& physicalDevice, 
    VkCommandPool& commandPool, 
    VkFormat& swapChainImageFormat, 
    std::vector<VkImage>& swapChainImages, 
    std::vector<VkImageView>& swapChainImageViews, 
    VkPipelineLayout& pipelineLayout,
    VkBuffer vertexBuffer,
    const std::vector<Vertex>& vertices, 
    VkBuffer indexBuffer,
    VkDescriptorSetLayout descriptorSetLayout,
    std::vector<void*>& uniformBuffersMapped,
    std::vector<VkDescriptorSet>& descriptorSets,
    VkImage depthImage,
    VkDeviceMemory depthImageMemory,
    VkImageView depthImageView,
    std::vector<VkFence>& imagesInFlight
    ) {
    vkWaitForFences(device, 1, &inFlightFences[Renderer.currentFrame], VK_TRUE, UINT64_MAX);
    
    VkResult result = vkAcquireNextImageKHR(
        device,
        swapChain,
        UINT64_MAX,
        imageAvailableSemaphores[Renderer.currentFrame],
        VK_NULL_HANDLE,
        &Renderer.imageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain(window, device, physicalDevice, surface, swapChain, MAX_FRAMES_IN_FLIGHT, renderPass, commandPool, commandBuffers, swapChainExtent, swapChainImages, swapChainImageFormat, swapChainImageViews, swapChainFramebuffers, graphicsPipeline, pipelineLayout, vertexBuffer, vertices, indexBuffer, Renderer.indices, descriptorSetLayout, descriptorSets, Renderer.currentFrame, graphicsQueue, depthImageView, depthImageMemory, depthImage);
        imagesInFlight.clear();
        imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    if (imagesInFlight[Renderer.imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(
            device,
            1,
            &imagesInFlight[Renderer.imageIndex],
            VK_TRUE,
            UINT64_MAX
        );
    }

    vkResetCommandBuffer(
        commandBuffers[Renderer.imageIndex],
        0
    );

    recordCommandBuffer(); 


    imagesInFlight[Renderer.imageIndex] = inFlightFences[Renderer.currentFrame];

    updateUniformBuffer(Renderer.currentFrame, uniformBuffersMapped, swapChainExtent);

    vkResetFences(device, 1, &inFlightFences[Renderer.currentFrame]);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;


    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[Renderer.currentFrame] };
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[Renderer.imageIndex];


    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[Renderer.currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(
        graphicsQueue,
        1,
        &submitInfo,
        inFlightFences[Renderer.currentFrame]
    ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &Renderer.imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR ||
        result == VK_SUBOPTIMAL_KHR ||
        framebufferResized) {

        framebufferResized = false;
        recreateSwapChain(window, device, physicalDevice, surface, swapChain, MAX_FRAMES_IN_FLIGHT, renderPass, commandPool, commandBuffers, swapChainExtent, swapChainImages, swapChainImageFormat, swapChainImageViews, swapChainFramebuffers, graphicsPipeline, pipelineLayout, vertexBuffer, vertices, indexBuffer, Renderer.indices, descriptorSetLayout, descriptorSets, Renderer.currentFrame, graphicsQueue, depthImageView, depthImageMemory, depthImage);
        imagesInFlight.clear();
        imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
        return;
    }

    Renderer.currentFrame = (Renderer.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

int main() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW!");
    } else {
        std::cout << "Initialized GLFW correctly!" << std::endl;
    }

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    createWindow();
    glfwSetFramebufferSizeCallback(Window.window, framebufferResizeCallback);
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    imagesInFlight.clear();
    imagesInFlight.resize(SwapChain.swapChainImages.size(), VK_NULL_HANDLE);
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createDepthResources();
    createFramebuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createVertexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createIndexBuffer();
    createCommandBuffer();   
    for (size_t i = 0; i < Buffers.commandBuffers.size(); i++) {
        recordCommandBuffer();
    }
    createSyncObjects(Device.device, imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences, Renderer.MAX_FRAMES_IN_FLIGHT);  

    // Main Loop
    while (!glfwWindowShouldClose(Window.window)) {
        glfwPollEvents();
        drawFrame(Device.device, SwapChain.swapChain, Pipeline.renderPass, SwapChain.swapChainFramebuffers, SwapChain.swapChainExtent, Pipeline.graphicsPipeline, Queues.graphicsQueue, Queues.presentQueue, inFlightFences, renderFinishedSemaphores, imageAvailableSemaphores, Buffers.commandBuffers, Renderer.MAX_FRAMES_IN_FLIGHT, Surface.surface, Window.window, Device.physicalDevice, Device.commandPool, SwapChain.swapChainImageFormat, SwapChain.swapChainImages, SwapChain.swapChainImageViews, Pipeline.pipelineLayout, VertexBuffer.vertexBuffer, Renderer.vertices, IndexBuffer.indexBuffer, Pipeline.descriptorSetLayout, UniformBuffers.uniformBuffersMapped, Device.descriptorSets, DepthResources.depthImage, DepthResources.depthImageMemory, DepthResources.depthImageView, imagesInFlight);
    }

    // Cleanup
    // TODO: When closing the app after resizing/changing size of the window, validation layers scream
    vkDeviceWaitIdle(Device.device);   
    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(Device.device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Device.device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(Device.device, inFlightFences[i], nullptr);
    }
    for (auto framebuffer : SwapChain.swapChainFramebuffers) {
        vkDestroyFramebuffer(Device.device, framebuffer, nullptr);
    }
    SwapChain.swapChainFramebuffers.clear();
    vkFreeCommandBuffers(
        Device.device,
        Device.commandPool,
        static_cast<uint32_t>(Buffers.commandBuffers.size()),
        Buffers.commandBuffers.data()
    );
    Buffers.commandBuffers.clear();
    vkDestroyCommandPool(Device.device, Device.commandPool, nullptr);
    for (auto imageView : SwapChain.swapChainImageViews) {
        vkDestroyImageView(Device.device, imageView, nullptr);
    }
    SwapChain.swapChainImageViews.clear();
    cleanupSwapChain(Device.device, Pipeline.renderPass, SwapChain.swapChainFramebuffers, Device.commandPool, Buffers.commandBuffers, SwapChain.swapChainImageViews, SwapChain.swapChain, Pipeline.graphicsPipeline, Pipeline.pipelineLayout);
    vkDestroyImageView(Device.device, DepthResources.depthImageView, nullptr);
    vkDestroyImage(Device.device, DepthResources.depthImage, nullptr);
    vkFreeMemory(Device.device, DepthResources.depthImageMemory, nullptr);
    vkDestroySampler(Device.device, Image.textureSampler, nullptr);
    vkDestroyImageView(Device.device, Image.textureImageView, nullptr);
    vkDestroyImage(Device.device, Image.textureImage, nullptr);
    vkFreeMemory(Device.device, Image.textureImageMemory, nullptr);         
    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(Device.device, UniformBuffers.uniformBuffers[i], nullptr);
        vkFreeMemory(Device.device, UniformBuffers.uniformBuffersMemory[i], nullptr);
    }
    vkDestroyDescriptorPool(Device.device, Device.descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(Device.device, Pipeline.descriptorSetLayout, nullptr);
    vkDestroyBuffer(Device.device, VertexBuffer.vertexBuffer, nullptr);
    vkFreeMemory(Device.device, VertexBuffer.vertexBufferMemory, nullptr);
    vkDestroyBuffer(Device.device, IndexBuffer.indexBuffer, nullptr);
    vkFreeMemory(Device.device, IndexBuffer.indexBufferMemory, nullptr);
    vkDestroyDevice(Device.device, nullptr);
    vkDestroySurfaceKHR(Instance.instance, Surface.surface, nullptr);
    cleanupInstance(Instance.instance);
    glfwDestroyWindow(Window.window);
    glfwTerminate();
}

