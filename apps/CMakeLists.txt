add_executable(app app.cc)

apply_compiler_flags(app)

# required libraries
target_link_libraries(app PRIVATE fmt::fmt gui_library)

if (APPLE)
    target_link_libraries(app
        PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo")
endif()
