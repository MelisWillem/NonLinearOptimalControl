cmake_minimum_required(VERSION 3.20)
project(CPP_AD)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_SOURCE_DIR}/cmake_conan/conan.cmake)

conan_cmake_configure(REQUIRES
        catch2/2.13.6
        GENERATORS
        ycm
        cmake_find_package
        cmake
        cmake_paths)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Catch2 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/runtimeAd")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/optimalControl")
add_subdirectory("${CMAKE_SOURCE_DIR}/test/runtimeAdTests")
add_subdirectory("${CMAKE_SOURCE_DIR}/test/optimalControlTests")

enable_testing()

add_test(NAME solverTests COMMAND runtimeAdTests)
