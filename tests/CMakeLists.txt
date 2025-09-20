# Try to find Catch2 that is at least 3.8 but the same major version
find_package(Catch2 CONFIG 3.8...<4.0 QUIET)
if (Catch2_FOUND)
  message(STATUS "Found Catch2 version: ${Catch2_VERSION}")
else()
  message(STATUS "Catch2 could not be found or version is too old, downloading via FetchContent...")
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1)
  FetchContent_MakeAvailable(Catch2)
endif()

# add_executable(mathtestlib mathtestlib.cc)

# target_link_libraries(mathtestlib PRIVATE Catch2::Catch2WithMain math_library)

# add_test(NAME mathtestlibtest COMMAND mathtestlib)
