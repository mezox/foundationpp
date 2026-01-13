# StdModuleProvider.cmake
# Reusable CMake module to provide C++23 std module via multiple backends
#
# Creates target: std
#
# Configuration variables (set before including this file):
#   STD_MODULE_PROVIDER  - "auto", "cmake_experimental", "libcxx_manual", "libcxx_system"
#   LIBCXX_MODULE_PATH   - Path to directory containing std.cppm (for libcxx_manual)
#   LIBCXX_INCLUDE_PATH  - Path to libc++ headers (optional, for libcxx_manual/libcxx_system)
#
# Example usage:
#   set(LIBCXX_MODULE_PATH "/path/to/llvm/modules/c++/v1")
#   include(StdModuleProvider)
#   target_link_libraries(mylib PUBLIC std)

include_guard(GLOBAL)

set(STD_MODULE_PROVIDER "auto" CACHE STRING "How to provide std module (auto, cmake_experimental, libcxx_manual, libcxx_system)")
set_property(CACHE STD_MODULE_PROVIDER PROPERTY STRINGS auto cmake_experimental libcxx_manual libcxx_system)

set(LIBCXX_MODULE_PATH "" CACHE PATH "Path to directory containing std.cppm (for libcxx_manual provider)")
set(LIBCXX_INCLUDE_PATH "" CACHE PATH "Path to libc++ headers (optional)")

# Backend 1: CMake experimental CXX_MODULE_STD (CMake 3.30+)
function(_std_module_try_cmake_experimental result)
    # Check CMake version
    if(CMAKE_VERSION VERSION_LESS "3.30")
        set(${result} FALSE PARENT_SCOPE)
        return()
    endif()

    # Check if compiler supports import std for C++23
    # CMAKE_CXX_COMPILER_IMPORT_STD is set after project() when compiler is detected
    if(NOT DEFINED CMAKE_CXX_COMPILER_IMPORT_STD)
        set(${result} FALSE PARENT_SCOPE)
        return()
    endif()

    if("23" IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
        set(${result} TRUE PARENT_SCOPE)
    else()
        set(${result} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Backend 2: Manual libc++ path
function(_std_module_try_libcxx_manual result)
    if(NOT LIBCXX_MODULE_PATH)
        set(${result} FALSE PARENT_SCOPE)
        return()
    endif()

    if(EXISTS "${LIBCXX_MODULE_PATH}/std.cppm")
        set(${result} TRUE PARENT_SCOPE)
    else()
        message(WARNING "LIBCXX_MODULE_PATH set but std.cppm not found at: ${LIBCXX_MODULE_PATH}/std.cppm")
        set(${result} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Backend 3: Search system paths for libc++ modules
function(_std_module_try_libcxx_system result found_path)
    # Common paths where libc++ modules might be installed
    set(search_paths
        # Linux - LLVM packages
        "/usr/lib/llvm-20/share/libc++/v1"
        "/usr/lib/llvm-19/share/libc++/v1"
        "/usr/lib/llvm-18/share/libc++/v1"
        "/usr/lib/llvm-17/share/libc++/v1"
        # Linux - generic
        "/usr/share/libc++/v1"
        "/usr/local/share/libc++/v1"
        # macOS - Homebrew Apple Silicon
        "/opt/homebrew/opt/llvm/share/libc++/v1"
        # macOS - Homebrew Intel
        "/usr/local/opt/llvm/share/libc++/v1"
        # macOS - MacPorts
        "/opt/local/libexec/llvm-19/share/libc++/v1"
        "/opt/local/libexec/llvm-18/share/libc++/v1"
    )

    foreach(path IN LISTS search_paths)
        if(EXISTS "${path}/std.cppm")
            set(${result} TRUE PARENT_SCOPE)
            set(${found_path} "${path}" PARENT_SCOPE)
            message(STATUS "std module: found libc++ modules at ${path}")
            return()
        endif()
    endforeach()

    set(${result} FALSE PARENT_SCOPE)
    set(${found_path} "" PARENT_SCOPE)
endfunction()

# Create std target using cmake_experimental backend
function(_std_module_create_cmake_experimental)
    # The experimental UUID changes between CMake versions
    # This is the UUID for CMake 3.30+
    # See: https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508" CACHE STRING "" FORCE)

    # Create an interface library that consumers link to
    # CMake will automatically handle std module for targets with CXX_MODULE_STD ON
    add_library(std INTERFACE)
    set_target_properties(std PROPERTIES
        INTERFACE_CXX_MODULE_STD ON
    )

    message(STATUS "std module: using CMake experimental CXX_MODULE_STD")
endfunction()

# Create std target using libc++ backend
function(_std_module_create_libcxx module_path include_path)
    add_library(std STATIC)
    target_sources(std
        PUBLIC FILE_SET CXX_MODULES TYPE CXX_MODULES
        BASE_DIRS "${module_path}"
        FILES "${module_path}/std.cppm"
    )
    target_compile_options(std
        PUBLIC
            -Wno-reserved-module-identifier
            -Wno-reserved-user-defined-literal
    )
    if(include_path AND EXISTS "${include_path}")
        target_include_directories(std SYSTEM PUBLIC "${include_path}")
    endif()
    set_target_properties(std PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_SCAN_FOR_MODULES ON
    )

    message(STATUS "std module: using libc++ from ${module_path}")
endfunction()

# Main logic - determine provider and create std target
if(TARGET std)
    message(STATUS "std module: target already exists, skipping")
elseif(STD_MODULE_PROVIDER STREQUAL "auto")
    # Try backends in priority order
    _std_module_try_cmake_experimental(_cmake_ok)
    if(_cmake_ok)
        _std_module_create_cmake_experimental()
    else()
        _std_module_try_libcxx_manual(_manual_ok)
        if(_manual_ok)
            _std_module_create_libcxx("${LIBCXX_MODULE_PATH}" "${LIBCXX_INCLUDE_PATH}")
        else()
            _std_module_try_libcxx_system(_system_ok _system_path)
            if(_system_ok)
                _std_module_create_libcxx("${_system_path}" "${LIBCXX_INCLUDE_PATH}")
            else()
                message(FATAL_ERROR
                    "StdModuleProvider: Could not find std module provider.\n"
                    "Options:\n"
                    "  1. Use CMake 3.30+ with a compiler supporting 'import std'\n"
                    "  2. Set LIBCXX_MODULE_PATH to directory containing std.cppm\n"
                    "  3. Install libc++ with modules enabled to a system path\n"
                    "\n"
                    "Example:\n"
                    "  cmake -DLIBCXX_MODULE_PATH=/path/to/llvm/modules/c++/v1 .."
                )
            endif()
        endif()
    endif()
elseif(STD_MODULE_PROVIDER STREQUAL "cmake_experimental")
    _std_module_try_cmake_experimental(_ok)
    if(NOT _ok)
        message(FATAL_ERROR
            "StdModuleProvider: cmake_experimental provider requires:\n"
            "  - CMake 3.30 or later\n"
            "  - Compiler with C++23 'import std' support\n"
            "Current CMake version: ${CMAKE_VERSION}"
        )
    endif()
    _std_module_create_cmake_experimental()
elseif(STD_MODULE_PROVIDER STREQUAL "libcxx_manual")
    if(NOT LIBCXX_MODULE_PATH)
        message(FATAL_ERROR
            "StdModuleProvider: libcxx_manual provider requires LIBCXX_MODULE_PATH to be set.\n"
            "Example: cmake -DLIBCXX_MODULE_PATH=/path/to/llvm/modules/c++/v1 .."
        )
    endif()
    if(NOT EXISTS "${LIBCXX_MODULE_PATH}/std.cppm")
        message(FATAL_ERROR
            "StdModuleProvider: std.cppm not found at: ${LIBCXX_MODULE_PATH}/std.cppm\n"
            "Ensure LIBCXX_MODULE_PATH points to the directory containing std.cppm"
        )
    endif()
    _std_module_create_libcxx("${LIBCXX_MODULE_PATH}" "${LIBCXX_INCLUDE_PATH}")
elseif(STD_MODULE_PROVIDER STREQUAL "libcxx_system")
    _std_module_try_libcxx_system(_ok _path)
    if(NOT _ok)
        message(FATAL_ERROR
            "StdModuleProvider: libcxx_system provider could not find std.cppm in system paths.\n"
            "Install libc++ with modules enabled, or use libcxx_manual provider with explicit path."
        )
    endif()
    _std_module_create_libcxx("${_path}" "${LIBCXX_INCLUDE_PATH}")
else()
    message(FATAL_ERROR
        "StdModuleProvider: Unknown provider '${STD_MODULE_PROVIDER}'.\n"
        "Valid options: auto, cmake_experimental, libcxx_manual, libcxx_system"
    )
endif()
