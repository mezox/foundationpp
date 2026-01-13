cmake_minimum_required(VERSION 3.28)

project(foundationpp
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Modern C++23 module wrapper for Apple's Foundation framework"
)

# Validate platform - Foundation is Apple-only
if(NOT APPLE)
    message(FATAL_ERROR "foundationpp requires Apple platforms (macOS/iOS/tvOS/watchOS)")
endif()

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(FOUNDATIONPP_BUILD_SHARED "Build shared library" ON)
option(FOUNDATIONPP_BUILD_STATIC "Build static library" OFF)
option(FOUNDATIONPP_SAFE_MESSAGES "Enable runtime selector checks with assertions" OFF)

# Find required frameworks
find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
find_library(OBJC_LIBRARY objc REQUIRED)

# Provide std module (creates 'std' target via auto-detection or explicit configuration)
# See cmake/StdModuleProvider.cmake for configuration options
include(cmake/StdModuleProvider.cmake)

# Verify std target was created
if(NOT TARGET std)
    message(FATAL_ERROR "std target not found. StdModuleProvider failed to create it.")
endif()

# Build the modules
add_subdirectory(modules)
