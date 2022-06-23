add_library(optimalControl STATIC 
	"optimalControl.h"
	"optimalControl.cpp"
    "integrators/explicit_euler.h" 
	"systemBehavior.h" 
	"integrators/runga_kutta.h"
	"simulator.h"
	"constraints/weightedL2.h")

target_include_directories(optimalControl PRIVATE "${PROJECT_SOURCE_DIR}/src")
