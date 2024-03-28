#include<catch2/catch_test_macros.hpp>
#include<catch2/catch_approx.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Given_Sum_Check_Gradient")
{
	auto a = Var(0);
	auto b = Var(1);

	auto f = a + b;

	std::vector<double> x = { 1, 2 };
	std::vector<double> dx(2);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Catch::Approx(1.0).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
	auto expected_grad_1 = Catch::Approx(1.0).epsilon(1e-6);
	REQUIRE(dx[1] == expected_grad_1);
}