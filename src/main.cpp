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
#include "app.hpp"
#include "model.hpp"

// Document everything

int main() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW!");
    } else {
        std::cout << "Initialized GLFW correctly!" << std::endl;
    }

    createWindow();
    glfwSetFramebufferSizeCallback(Window.window, frameBufferResizeCallback);
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    Fences.imagesInFlight.clear();
    Fences.imagesInFlight.resize(SwapChain.swapChainImages.size(), VK_NULL_HANDLE);
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createDepthResources();
    createFramebuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    Model.loadModel();
    std::cout << "Vertices count: " << Renderer.vertices.size() << std::endl;
    std::cout << "Indices count: " << Renderer.indices.size() << std::endl;
    createVertexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createIndexBuffer();
    createCommandBuffer();  

    for (size_t i = 0; i < Buffers.commandBuffers.size(); i++) {
        recordCommandBuffer();
    }
    createSyncObjects();  

    // Main Loop
    while (!glfwWindowShouldClose(Window.window)) {
        glfwPollEvents();
        Renderer.drawFrame();
    }

    App.clean();
}