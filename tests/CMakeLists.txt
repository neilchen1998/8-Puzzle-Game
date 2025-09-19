FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1)
FetchContent_MakeAvailable(catch)

# add_executable(mathtestlib mathtestlib.cc)

# target_link_libraries(mathtestlib PRIVATE Catch2::Catch2WithMain math_library)

# add_test(NAME mathtestlibtest COMMAND mathtestlib)
