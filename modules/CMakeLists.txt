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

# Determine library type
if(FOUNDATIONPP_BUILD_SHARED)
    add_library(foundationpp SHARED)
else()
    add_library(foundationpp STATIC)
endif()

# Add module sources
target_sources(foundationpp
    PUBLIC FILE_SET CXX_MODULES TYPE CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${FOUNDATIONPP_PRIMARY_MODULE}
        ${FOUNDATIONPP_MODULE_PARTITIONS}
)

# Link dependencies
target_link_libraries(foundationpp
    PUBLIC
        std
        ${FOUNDATION_FRAMEWORK}
        ${OBJC_LIBRARY}
)

# Compiler options
target_compile_options(foundationpp
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-reserved-module-identifier
        -Wno-reserved-user-defined-literal
)

# Safe message dispatch option
if(FOUNDATIONPP_SAFE_MESSAGES)
    target_compile_definitions(foundationpp PRIVATE FOUNDATIONPP_SAFE_MESSAGES=1)
endif()

# Target properties
set_target_properties(foundationpp PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_SCAN_FOR_MODULES ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(FOUNDATIONPP_BUILD_SHARED)
    include(${CMAKE_SOURCE_DIR}/cmake/SymbolExports.cmake)
    add_symbol_exports(foundationpp
        "${CMAKE_CURRENT_SOURCE_DIR}/foundationpp.exports"
    )
endif()
