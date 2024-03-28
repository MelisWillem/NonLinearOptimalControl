add_executable(optimalControlTests cart.cpp)

target_link_libraries(optimalControlTests PUBLIC Catch2::Catch2WithMain)
target_include_directories(optimalControlTests PRIVATE "${PROJECT_SOURCE_DIR}/src")
