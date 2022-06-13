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