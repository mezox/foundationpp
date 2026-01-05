# SymbolExports.cmake
# Similar to LLVM's add_llvm_symbol_exports() function
# Provides cross-platform symbol export handling for shared libraries

# add_symbol_exports(target_name export_file)
#
# Configure a shared library target to only export symbols listed in the export file.
# The export file should contain one symbol per line (without leading underscore).
# Comments starting with # and empty lines are ignored.
#
# Platform-specific handling:
# - Darwin/macOS: Uses -exported_symbols_list linker flag
# - Linux/Unix: Uses version script with --version-script linker flag
#
function(add_symbol_exports target_name export_file)
    get_filename_component(export_file_abs "${export_file}" ABSOLUTE)

    if(NOT EXISTS "${export_file_abs}")
        message(FATAL_ERROR "Export file not found: ${export_file_abs}")
    endif()

    if(APPLE)
        # Darwin: Convert to native format (prepend underscore) and use -exported_symbols_list
        set(native_export_file "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.exports")

        # Use grep to filter out comments and empty lines, then sed to prepend underscore
        add_custom_command(OUTPUT "${native_export_file}"
            COMMAND sh -c "grep -v '^#' '${export_file_abs}' | grep -v '^$$' | grep '.' | sed 's/^/_/' > '${native_export_file}'"
            DEPENDS "${export_file_abs}"
            VERBATIM
            COMMENT "Creating Darwin export list for ${target_name}"
        )

        add_custom_target(${target_name}_exports DEPENDS "${native_export_file}")
        add_dependencies(${target_name} ${target_name}_exports)

        target_link_options(${target_name} PRIVATE
            "-Wl,-exported_symbols_list,${native_export_file}"
        )

    elseif(UNIX)
        # Linux/Unix: Convert to version script format
        set(native_export_file "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.ver")

        add_custom_command(OUTPUT "${native_export_file}"
            COMMAND ${CMAKE_COMMAND} -E echo "{ global:" > "${native_export_file}"
            COMMAND sed -e "/^#/d" -e "/^[[:space:]]*$$/d" -e "s/$$/;/"
                    < "${export_file_abs}" >> "${native_export_file}"
            COMMAND ${CMAKE_COMMAND} -E echo "local: *; };" >> "${native_export_file}"
            DEPENDS "${export_file_abs}"
            VERBATIM
            COMMENT "Creating version script for ${target_name}"
        )

        add_custom_target(${target_name}_exports DEPENDS "${native_export_file}")
        add_dependencies(${target_name} ${target_name}_exports)

        target_link_options(${target_name} PRIVATE
            "-Wl,--version-script,${native_export_file}"
        )
    endif()
endfunction()
