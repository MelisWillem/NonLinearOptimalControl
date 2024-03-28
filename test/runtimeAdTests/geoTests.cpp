#include<catch2/catch_test_macros.hpp>
#include<catch2/catch_approx.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Given_Cos_Check_Gradient")
{
	auto a = Var(0);
	auto f = cos(a);

	std::vector<double> x = { 1 };
	std::vector<double> dx(1);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Catch::Approx(-sin(a->Value(x))).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
}

TEST_CASE("Given_Sin_Check_Gradient")
{
	auto a = Var(0);
	auto f = sin(a);

	std::vector<double> x = { 1 };
	std::vector<double> dx(1);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Catch::Approx(cos(a->Value(x))).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
}
