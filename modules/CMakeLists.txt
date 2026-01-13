# Module partition sources (order matters for dependency scanning)
set(FOUNDATIONPP_MODULE_PARTITIONS
    detail/detail.cppm
    detail/runtime.cppm
    types.cppm
    object.cppm
    memory.cppm
    string.cppm
    collections.cppm
    data.cppm
    error.cppm
    time.cppm
    bundle.cppm
    process.cppm
    concurrency.cppm
    notification.cppm
)

# Primary module interface
set(FOUNDATIONPP_PRIMARY_MODULE
    foundationpp.cppm
)

# Validate that at least one library type is enabled
if(NOT FOUNDATIONPP_BUILD_SHARED AND NOT FOUNDATIONPP_BUILD_STATIC)
    message(FATAL_ERROR "At least one of FOUNDATIONPP_BUILD_SHARED or FOUNDATIONPP_BUILD_STATIC must be enabled")
endif()

# Function to configure a foundationpp library target
function(configure_foundationpp_target target_name library_type)
    add_library(${target_name} ${library_type})

    # Add module sources
    target_sources(${target_name}
        PUBLIC FILE_SET CXX_MODULES TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${FOUNDATIONPP_PRIMARY_MODULE}
            ${FOUNDATIONPP_MODULE_PARTITIONS}
    )

    # Link dependencies
    target_link_libraries(${target_name}
        PUBLIC
            std
            ${FOUNDATION_FRAMEWORK}
            ${OBJC_LIBRARY}
    )

    # Compiler options
    target_compile_options(${target_name}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-reserved-module-identifier
            -Wno-reserved-user-defined-literal
    )

    # Safe message dispatch option
    if(FOUNDATIONPP_SAFE_MESSAGES)
        target_compile_definitions(${target_name} PRIVATE FOUNDATIONPP_SAFE_MESSAGES=1)
    endif()

    # Target properties
    set_target_properties(${target_name} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_SCAN_FOR_MODULES ON
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        OUTPUT_NAME foundationpp
    )

    # Symbol exports for shared libraries
    if(library_type STREQUAL "SHARED")
        include(${PROJECT_SOURCE_DIR}/cmake/SymbolExports.cmake)
        add_symbol_exports(${target_name}
            "${CMAKE_CURRENT_SOURCE_DIR}/foundationpp.exports"
        )
    endif()
endfunction()

# Build shared library
if(FOUNDATIONPP_BUILD_SHARED)
    configure_foundationpp_target(foundationpp_shared SHARED)
    add_library(foundationpp::shared ALIAS foundationpp_shared)
endif()

# Build static library
if(FOUNDATIONPP_BUILD_STATIC)
    configure_foundationpp_target(foundationpp_static STATIC)
    add_library(foundationpp::static ALIAS foundationpp_static)
endif()

# Create default alias (prefer shared if available)
if(FOUNDATIONPP_BUILD_SHARED)
    add_library(foundationpp ALIAS foundationpp_shared)
elseif(FOUNDATIONPP_BUILD_STATIC)
    add_library(foundationpp ALIAS foundationpp_static)
endif()
