#include<catch2/catch.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Give_Cost_Function_With_Same_Node_Twice_Compare_BackAD_Finite_diff")
{
	auto a = Var(0);
	auto b = Var(1);

	auto c = a+b;
	auto f = c*c;

	std::vector<double> x =	 {2.0, 3.0};

	std::vector<double> dx(std::size(x));
	EvaluateGradient(f, x, dx);

	std::vector<double> dxFiniteDifference(std::size(x));
	EvaluateGradientFiniteDifference(f, x, dxFiniteDifference);

	for(int i=0; i < std::size(dx); ++i)
	{
		REQUIRE(dx[i] == Approx(dxFiniteDifference[i]).epsilon(1e-4));
	}
}
