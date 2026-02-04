cmake_minimum_required(VERSION 3.21)

project(VulkanRenderer
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------- Vulkan ----------------
find_package(Vulkan REQUIRED)

# ---------------- Dependencies ----------------
add_subdirectory(external/GLFW)
add_subdirectory(external/glm)

# ---------------- ImGui ----------------
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    Vulkan::Vulkan
)

# ---------------- Sources ----------------
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# ---------------- Includes ----------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/external/stb
        ${PROJECT_SOURCE_DIR}/external/tinyobjloader
)

# ---------------- Link ----------------
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        imgui
        glfw
        Vulkan::Vulkan
        glm::glm
)

# ---------------- Platform ----------------
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE VK_USE_PLATFORM_WIN32_KHR
    )
endif()

# ---------------- Shaders ----------------
find_program(GLSL_VALIDATOR glslangValidator
    HINTS
        $ENV{VULKAN_SDK}/Bin
        $ENV{VULKAN_SDK}/Bin32
)

if (GLSL_VALIDATOR)
    file(GLOB_RECURSE GLSL_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/shaders/*.vert
        ${PROJECT_SOURCE_DIR}/shaders/*.frag
    )

    foreach(GLSL ${GLSL_SOURCE_FILES})
        get_filename_component(FILE_NAME ${GLSL} NAME)
        set(SPIRV ${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv)

        add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL}
        )

        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach()

    add_custom_target(Shaders DEPENDS ${SPIRV_BINARY_FILES})
    add_dependencies(${PROJECT_NAME} Shaders)
endif()
