FetchContent_Declare(
  nanobench
  GIT_REPOSITORY https://github.com/martinus/nanobench.git
  GIT_TAG v4.3.10)
FetchContent_MakeAvailable(nanobench)

add_executable(celebrationbenchmark celebrationbenchmark.cc)

# Link required libraries
target_link_libraries(celebrationbenchmark PRIVATE nanobench gui_library)
