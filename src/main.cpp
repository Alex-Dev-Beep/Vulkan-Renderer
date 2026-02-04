#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

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

static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

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

    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * 11;
    pool_info.poolSizeCount = 11;
    pool_info.pPoolSizes = pool_sizes;

    VkDescriptorPool imguiDescriptorPool;
    vkCreateDescriptorPool(Device.device, &pool_info, nullptr, &imguiDescriptorPool);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    QueueFamilyIndices indices = findQueueFamilies(Device.physicalDevice, Surface.surface);
    ImGui_ImplGlfw_InitForVulkan(Window.window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = Instance.instance;
    init_info.PhysicalDevice = Device.physicalDevice;
    init_info.Device = Device.device;
    init_info.QueueFamily = indices.graphicsFamily.value();
    init_info.Queue = Queues.graphicsQueue;
    init_info.PipelineCache = g_PipelineCache;
    init_info.DescriptorPool = imguiDescriptorPool;
    init_info.MinImageCount = static_cast<uint32_t>(SwapChain.swapChainImages.size());
    init_info.ImageCount = static_cast<uint32_t>(SwapChain.swapChainImages.size());
    init_info.Allocator = nullptr;
    init_info.PipelineInfoMain.RenderPass = Pipeline.renderPass;
    init_info.PipelineInfoMain.Subpass = 0;
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);
    
    createSyncObjects();  

    // Main Loop
    while (!glfwWindowShouldClose(Window.window)) {
        glfwPollEvents();
        Renderer.drawFrame();
    }

    App.clean();
}