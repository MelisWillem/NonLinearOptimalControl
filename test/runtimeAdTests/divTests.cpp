#include<catch2/catch.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Compare_Finite_Diff_With_BackProb_Div")
{
	auto a = Var(0);
	auto b = Var(1);

	auto f = a / b;

	std::vector<double> x = { 2, 3 };
	auto f_eval = EvaluateCost(f, x);

	std::vector<double> dx_backprop(2);
	EvaluateGradient(f, x, dx_backprop);
	std::vector<double> dx_finite_diff(2);
	EvaluateGradientFiniteDifference(f, x, dx_finite_diff);

	for (int i = 0; i < std::size(dx_backprop); i++)
	{
		REQUIRE(dx_backprop[i] == Approx(dx_finite_diff[i]).epsilon(1e-6));
	}
}
