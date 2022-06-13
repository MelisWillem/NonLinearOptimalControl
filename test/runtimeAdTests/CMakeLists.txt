add_executable(runtimeAdTests main.cpp sumTests.cpp prodTests.cpp geoTests.cpp wikiExampleTests.cpp)
target_link_libraries(runtimeAdTests PUBLIC Catch2::Catch2)

target_include_directories(runtimeAdTests PRIVATE "${PROJECT_SOURCE_DIR}/src")