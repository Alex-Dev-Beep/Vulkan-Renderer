#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

void createSwapChain(VkPhysicalDevice physicalDevice, 
    VkSurfaceKHR surface, 
    GLFWwindow* window, 
    VkSwapchainKHR& swapChain, 
    VkDevice device, 
    std::vector<VkImage>& swapChainImages, 
    VkFormat& swapChainImageFormat, 
    VkExtent2D& swapChainExtent
);

void createImageViews(std::vector<VkImageView>& swapChainImageViews, std::vector<VkImage>& swapChainImages, VkFormat swapChainImageFormat, VkDevice device);