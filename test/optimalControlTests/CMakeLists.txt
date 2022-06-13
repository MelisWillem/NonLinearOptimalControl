add_executable(optimalControlTests main.cpp)

target_link_libraries(optimalControlTests PUBLIC Catch2::Catch2)
target_include_directories(optimalControlTests PRIVATE "${PROJECT_SOURCE_DIR}/src")
