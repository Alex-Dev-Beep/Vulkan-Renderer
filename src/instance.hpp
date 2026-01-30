#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void createInstance();
bool checkValidationLayerSupport();

void setupDebugMessenger();

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* messenger
);

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* allocator
);

void cleanupInstance(VkInstance instance);

struct instance {
    public:
    VkInstance instance;
};

extern instance Instance;