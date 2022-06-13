#include<catch2/catch.hpp>
#include<runtimeAd/runtimeAd.h>

using namespace runtimeAd;

TEST_CASE("Given_Cos_Check_Gradient")
{
	auto a = Var(0);
	auto f = cos(a);

	std::vector<double> x = { 1 };
	std::vector<double> dx(1);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Approx(-sin(a->Value(x))).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
}

TEST_CASE("Given_Sin_Check_Gradient")
{
	auto a = Var(0);
	auto f = sin(a);

	std::vector<double> x = { 1 };
	std::vector<double> dx(1);
	EvaluateGradient(f, x, dx);

	auto expected_grad_0 = Approx(cos(a->Value(x))).epsilon(1e-6);
	REQUIRE(dx[0] == expected_grad_0);
}
