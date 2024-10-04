cmake_minimum_required(VERSION 3.30)

include(cmake/enable_import_std.cmake)

project(LearningVulkan CXX)

find_package(VulkanLoader CONFIG REQUIRED)
find_package(vulkan-validationlayers CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

include(cmake/common.cmake)

set(WITH_CLANG_TIDY OFF)

if (WITH_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif ()

add_subdirectory(ch02)
