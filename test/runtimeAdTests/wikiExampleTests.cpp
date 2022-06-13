#include<catch2/catch.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Given_Example_Wiki_Check_Gradient")
{
	auto x1 = Var(0);
	auto x2 = Var(1);

	auto f = (x1 * x2) + sin(x1);

	std::vector<double> x = { 1, 2 };
	std::vector<double> dx(2);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = std::cos(x1->Value(x)) + x2->Value(x);
	auto approx_expected_grad_0 = Approx(expected_grad_0)
		.epsilon(1e-6);
	REQUIRE(dx[0] == approx_expected_grad_0);

	auto expected_grad_1 = x1->Value(x);
	auto approx_expected_grad_1 = Approx(expected_grad_1)
		.epsilon(1e-6);
	REQUIRE(dx[1] == approx_expected_grad_1);
}
