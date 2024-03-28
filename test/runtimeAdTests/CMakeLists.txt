add_executable(runtimeAdTests 
	sumTests.cpp 
	prodTests.cpp 
	geoTests.cpp 
	wikiExampleTests.cpp 
	divTests.cpp 
	absTests.cpp
	recursiveFunctionTests.cpp)
target_link_libraries(runtimeAdTests PUBLIC Catch2::Catch2WithMain)

target_include_directories(runtimeAdTests PRIVATE "${PROJECT_SOURCE_DIR}/src")
