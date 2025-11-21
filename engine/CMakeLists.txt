file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
    VulkanMemoryAllocator
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG master
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)

FetchContent_MakeAvailable(VulkanMemoryAllocator)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)

add_library(engine STATIC)

target_sources(engine PRIVATE ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC
    "src/include"
)

target_link_libraries(engine PRIVATE
    glfw
    glm::glm
    Vulkan::Vulkan
    GPUOpen::VulkanMemoryAllocator
)

enable_project_warnings(engine)