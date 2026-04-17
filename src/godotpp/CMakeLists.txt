# Define C++ GDExtension Library
add_library(GodotVehicles SHARED
        src/gd_example.cpp
        src/register_types.cpp
        include/parts/wheel.h
        src/parts/wheel.cpp
        include/nodes/vehicles.h
        src/nodes/vehicles.cpp
)
target_link_libraries(GodotVehicles PRIVATE
        godot::cpp
)
target_include_directories(GodotVehicles PUBLIC include)

# The "Install" Step (Copy Lib + Generate .gdextension)
add_custom_command(TARGET GodotVehicles POST_BUILD
        # Copy the binary to the Godot project bin/ folder
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GODOT_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:GodotVehicles>" "${GODOT_OUTPUT_DIR}"

        # BGenerate the .gdextension file from template
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/godotvehicles.gdextension.in"
        "${GODOT_PROJECT_DIR}/godotvehicles.gdextension"

        COMMENT "Installing GDExtension to ${GODOT_OUTPUT_DIR}..."
)
