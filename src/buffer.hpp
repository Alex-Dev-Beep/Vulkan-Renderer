#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct vertexBuffer {
    public:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
};

struct indexBuffer {
    public:
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
};

extern vertexBuffer VertexBuffer;
extern indexBuffer IndexBuffer;