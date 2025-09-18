cmake_minimum_required(VERSION 3.21)

project(game LANGUAGES CXX)

file(GLOB_RECURSE CPP_SOURCES "source/*.cpp")

find_package(VulkanHeaders CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)

if (APPLE)
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)
    find_package(spirv_cross_msl CONFIG REQUIRED)
    file(GLOB_RECURSE MM_SOURCES "source/*.mm")
endif()

if (WIN32)
    find_package(spirv_cross_hlsl CONFIG REQUIRED)
endif()
  
add_executable(game)

target_compile_options(game PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -O0>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /Od>
)

target_link_libraries(game PRIVATE 
    glfw
    Vulkan::Headers
    glm::glm
    spirv-cross-core
)

if(APPLE)
    target_sources(game PRIVATE ${MM_SOURCES})
    target_link_libraries(game PRIVATE ${METAL_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} spirv-cross-msl)
endif()

if (WIN32)
    target_link_libraries(game PRIVATE spirv-cross-hlsl)
endif()

target_sources(game PRIVATE ${CPP_SOURCES})

target_include_directories(game PRIVATE "include")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(game PRIVATE DEBUG_BUILD_TYPE)
endif()

if(UNIX)
    target_compile_definitions(game PRIVATE PLATFORM_UNIX)
endif()

if(APPLE)
    target_compile_definitions(game PRIVATE PLATFORM_APPLE)
elseif(WIN32)
    target_compile_definitions(game PRIVATE PLATFORM_WINDOWS)
endif()