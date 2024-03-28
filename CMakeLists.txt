cmake_minimum_required(VERSION 3.20)
project(CPP_AD)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.3
)
FetchContent_MakeAvailable(Catch2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/runtimeAd")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/optimalControl")
add_subdirectory("${CMAKE_SOURCE_DIR}/test/runtimeAdTests")
add_subdirectory("${CMAKE_SOURCE_DIR}/test/optimalControlTests")

enable_testing()

add_test(NAME runtimeAdTests COMMAND runtimeAdTests)
add_test(NAME optimalControlTests COMMAND optimalControlTests)
