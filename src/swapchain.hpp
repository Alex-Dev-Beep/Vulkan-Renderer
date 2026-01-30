#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vertex.hpp"

#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct swapChain {
    public:
    VkSwapchainKHR swapChain;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

void createSwapChain();

void createImageViews();
void cleanupSwapChain(VkDevice& device, VkRenderPass& renderPass, std::vector<VkFramebuffer>& swapChainFramebuffers, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkImageView>& swapChainImageViews, VkSwapchainKHR& swapChain, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout);
void recreateSwapChain(GLFWwindow* window, VkDevice& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, int MAX_FRAMES_IN_FLIGHT, VkRenderPass& renderPass, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, VkExtent2D& swapChainExtent, std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, std::vector<VkImageView>& swapChainImageViews, std::vector<VkFramebuffer>& swapChainFramebuffers, VkPipeline& graphicsPipeline, VkPipelineLayout& pipelineLayout, VkBuffer vertexBuffer, const std::vector<Vertex>& vertices, VkBuffer indexBuffer, const std::vector<uint16_t> indices, VkDescriptorSetLayout descriptorSetLayout, std::vector<VkDescriptorSet> descriptorSets, uint32_t currentFrame, VkQueue graphicsQueue, VkImageView depthImageView, VkDeviceMemory depthImageMemory, VkImage depthImage);

extern swapChain SwapChain;