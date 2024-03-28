#include<catch2/catch_test_macros.hpp>
#include<catch2/catch_approx.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Compare_Finite_Diff_With_Back_Prob_Abs")
{
	auto a = Var(0);

	auto f = Abs(a);

	std::vector<double> x = { 2 };
	auto f_eval = EvaluateCost(f, x);

	std::vector<double> dx_backprop(1);
	EvaluateGradient(f, x, dx_backprop);
	std::vector<double> dx_finite_diff(1);
	EvaluateGradientFiniteDifference(f, x, dx_finite_diff);

	REQUIRE(dx_backprop[0] == Catch::Approx(dx_finite_diff[0]).epsilon(1e-6));
}
