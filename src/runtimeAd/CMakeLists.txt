add_library(runtimeAd STATIC 
	"runtimeAd.h"
	"runtimeAd.cpp"
	"expression/variable.h"
	"expression/constant.h"
	"expression/expression.h"
    "expression/sum.h"
	"expression/prod.h" "expression/operators.h" "expression/geometric.h" "expression/sub.h" "expression/abs.h")

target_include_directories(runtimeAd PRIVATE "${PROJECT_SOURCE_DIR}/src")
