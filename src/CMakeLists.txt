# raylib
find_package(raylib CONFIG 5.5 QUIET)
if (NOT raylib_FOUND)
    message(STATUS "raylib could not be found or version is too old, downloading via FetchContent...")
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
      raylib
      GIT_REPOSITORY https://github.com/raysan5/raylib
      GIT_TAG        5.5)
    FetchContent_MakeAvailable(raylib)
endif()

# slidr
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  slidr
  GIT_REPOSITORY https://github.com/neilchen1998/slidr
  GIT_TAG        v2.1.3)
FetchContent_MakeAvailable(slidr)

file(GLOB GUI_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/gui/*.hpp")

add_library(gui_library screenlib.cc animationlib.cc boardlib.cc celebration.cc menulib.cc settingslib.cc ${GUI_HEADER_LIST})

apply_compiler_flags(gui_library)

target_include_directories(gui_library PUBLIC ../include)

target_link_libraries(gui_library PUBLIC raylib fmt::fmt Slidr::slidr)

target_compile_features(gui_library PUBLIC cxx_std_23)  # requires C++23 for std::to_underlying
