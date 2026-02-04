#include "renderer.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

renderer Renderer;

void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
    Renderer.framebufferResized = true;
}

void renderer::drawFrame() {
    VkResult res;


    res = vkWaitForFences(
        Device.device,
        1,
        &Fences.inFlightFences[currentFrame],
        VK_TRUE,
        UINT64_MAX
    );


    res = vkAcquireNextImageKHR(
        Device.device,
        SwapChain.swapChain,
        UINT64_MAX,
        Semaphores.imageAvailableSemaphores[currentFrame],
        VK_NULL_HANDLE,
        &imageIndex
    );

    if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        std::cerr << "Acquire failed" << std::endl;
        return;
    }

    if (Fences.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(
            Device.device,
            1,
            &Fences.imagesInFlight[imageIndex],
            VK_TRUE,
            UINT64_MAX
        );
    }

    Fences.imagesInFlight[imageIndex] =
        Fences.inFlightFences[currentFrame];

    vkResetFences(Device.device, 1, &Fences.inFlightFences[currentFrame]);
    vkResetCommandBuffer(Buffers.commandBuffers[imageIndex], 0);

    updateUniformBuffer(Renderer.imageIndex, UniformBuffers.uniformBuffersMapped, SwapChain.swapChainExtent);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Text("HELLO DEBUG");
    ImGui::Render();

    recordCommandBuffer(imageIndex);


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {
        Semaphores.imageAvailableSemaphores[currentFrame]
    };
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Buffers.commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {
        Semaphores.renderFinishedSemaphores[currentFrame]
    };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    res = vkQueueSubmit(
        Queues.graphicsQueue,
        1,
        &submitInfo,
        Fences.inFlightFences[currentFrame]
    );
    std::cout << "Queue submit: " << res << std::endl;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &SwapChain.swapChain;
    presentInfo.pImageIndices = &imageIndex;

    res = vkQueuePresentKHR(
        Queues.presentQueue,
        &presentInfo
    );
    std::cout << "Present: " << res << std::endl;

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
