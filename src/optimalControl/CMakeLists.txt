add_library(optimalControl STATIC 
	"optimalControl.h"
	"optimalControl.cpp"
     )

target_include_directories(optimalControl PRIVATE "${PROJECT_SOURCE_DIR}/src")
