#include "app.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "swapchain.hpp"
#include "uniform.hpp"
#include "image.hpp"
#include "buffer.hpp"
#include "instance.hpp"
#include "pipeline.hpp"
#include "surface.hpp"
#include "sync.hpp"
#include "vertex.hpp"
#include "window.hpp"

app App;

void app::clean() {
    // Cleanup
    // TODO: When closing the app after resizing/changing size of the window, validation layers scream
    vkDeviceWaitIdle(Device.device);   
    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(Device.device, Semaphores.renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Device.device, Semaphores.imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(Device.device, Fences.inFlightFences[i], nullptr);
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