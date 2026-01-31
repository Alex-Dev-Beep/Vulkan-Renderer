#include "sync.hpp"
#include "device.hpp"
#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

void createSyncObjects() {
    Semaphores.imageAvailableSemaphores.resize(Renderer.MAX_FRAMES_IN_FLIGHT);
    Semaphores.renderFinishedSemaphores.resize(Renderer.MAX_FRAMES_IN_FLIGHT);
    Fences.inFlightFences.resize(Renderer.MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(Device.device, &semaphoreInfo, nullptr, &Semaphores.imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Device.device, &semaphoreInfo, nullptr, &Semaphores.renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Device.device, &fenceInfo, nullptr, &Fences.inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create sync objects!");
        }
    }

    std::cout << "Successfully created sync objects!" << std::endl;
}
