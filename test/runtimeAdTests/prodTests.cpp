#include<catch2/catch.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Given_Prod_Check_Gradient")
{
	auto a = Var(0);
	auto b = Var(1);

	auto f = a * b;

	std::vector<double> x = { 1, 2 };
	std::vector<double> dx(2);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Approx(b->Value(x)).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
	auto expected_grad_1 = Approx(a->Value(x)).epsilon(1e-6);
	REQUIRE(dx[1] == expected_grad_1);
}

TEST_CASE("Compare_Finite_Diff_With_Back_Prob_Prod")
{
	auto a = Var(0);
	auto b = Var(1);

	auto f = a * a + Constant(2) * b + a * b;

	std::vector<double> x = { 2, 3 };
	auto f_eval = EvaluateCost(f, x);

	REQUIRE(f_eval == Approx(2 * 2 + 2 * 3 + 2 * 3));

	std::vector<double> dx_backprop(2);
	EvaluateGradient(f, x, dx_backprop);
	std::vector<double> dx_finite_diff(2);
	EvaluateGradientFiniteDifference(f, x, dx_finite_diff);

	for (int i = 0; i < std::size(dx_backprop); i++)
	{
		REQUIRE(dx_backprop[i] == Approx(dx_finite_diff[i]).epsilon(1e-6));
	}
}
