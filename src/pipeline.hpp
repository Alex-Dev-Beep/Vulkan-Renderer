#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct pipeline {
    public:
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;
};

void createGraphicsPipeline();
void createRenderPass();
void createFramebuffers(
    std::vector<VkFramebuffer>& swapChainFramebuffers,
    const std::vector<VkImageView>& swapChainImageViews,
    VkRenderPass renderPass,
    VkExtent2D swapChainExtent,
    VkDevice device,
    VkImageView depthImageView
);

extern pipeline Pipeline;