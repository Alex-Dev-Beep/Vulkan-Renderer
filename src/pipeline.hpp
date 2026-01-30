#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct pipeline {
    public:
    VkRenderPass renderPass;
};

void createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkPipelineLayout& pipelineLayout, VkRenderPass renderPass, VkPipeline& graphicsPipeline, VkDescriptorSetLayout descriptorSetLayout);
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