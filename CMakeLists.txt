cmake_minimum_required(VERSION 3.21)

project(game LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.mm")

find_package(VulkanHeaders CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(game)

target_compile_options(game PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -O0>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /Od>
)

target_link_libraries(game PRIVATE 
    glfw
    GLEW::GLEW
    Vulkan::Headers
    glm::glm
)

if(APPLE)
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)
    target_link_libraries(game PRIVATE
        ${METAL_FRAMEWORK}
        ${QUARTZCORE_FRAMEWORK}
    )
endif()

target_sources(game PRIVATE ${SOURCES})

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
else()
    target_compile_definitions(game PRIVATE PLATFORM_UNKNOWN)
endif()