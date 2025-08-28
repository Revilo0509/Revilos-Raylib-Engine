if(NOT BUILD_EXAMPLES)
    return()
endif()

# Output directory for example binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples")

# Find all example source files
file(GLOB EXAMPLE_SOURCES "*.cpp")

# Path to resources
set(DEST_RESOURCES "${CMAKE_BINARY_DIR}/bin/examples/resources")

# Always create a custom target to copy resources
add_custom_target(copy_example_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${EXAMPLE_RESOURCES}"
    "${DEST_RESOURCES}"
    COMMENT "Copying example resources to bin/examples/resources"
)

# Create an executable for each example
foreach(EXAMPLE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)

    add_executable(${EXAMPLE_NAME} ${EXAMPLE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE RRE)

    # Make each example depend on the resource copy target
    add_dependencies(${EXAMPLE_NAME} copy_example_resources)

    # Copy the DLL to the same folder as the executable
    if(WIN32)
        add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:RRE>
            $<TARGET_FILE_DIR:${EXAMPLE_NAME}>
            COMMENT "Copying RRE DLL to example output directory"
        )
    endif()
endforeach()
