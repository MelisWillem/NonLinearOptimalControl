#include<optimalControl/optimalControl.h>
#include<catch2/catch.hpp>
#include<numeric>
#include<iostream>

// # Symbols / expressions
constexpr int number_of_states = 4;
constexpr int number_of_inputs = 2;

// # The system behavior :
// # dot_x0 = x_2
// # dot_x1 = x_3
// # dot_x2 = u_0 / m
// # dot_x3 = u_1 / m
class CartBehavior : public optimalControl::ContiniousSystemBehavior
{
	virtual multi_expr dot(multi_expr& current_state, multi_expr& current_input) override {
		using namespace runtimeAd;
		const double m = 1;
		// TODO::implement devision on runtimeAd
		auto inv_var_m = runtimeAd::Constant(1 / m);

		multi_expr next_state;
		next_state.push_back(current_state[2]);
		next_state.push_back(current_state[3]);
		next_state.push_back(current_input[0] * inv_var_m);
		next_state.push_back(current_input[1] * inv_var_m);

		return current_state;
	}
};

TEST_CASE("Give_Simpl_Cart_System_Move_To_Reference_Single_Shot")
{
	CartBehavior cont_system;

	double step_size = 1e-1;
	auto discrete_system = optimalControl::IntegrateWithExplicitEuler(cont_system, step_size);

	optimalControl::SingleShot::InitParams params;
	params.num_of_input = 2; // 2D motoro speed
	params.num_of_states = 4; // 2D position + speed of body
	params.num_of_steps_horizon = 100;
	params.system = &discrete_system;

	optimalControl::SingleShot problem(params);
	auto s = problem.GradientSize();

	std::vector<double> current_input_horizon(s, 0);

	int num_steps = 10;
	const double start_cost = problem.Cost(current_input_horizon);
	double step_rate = 0.1;
	std::cout << "Starting with a cost = " << start_cost << std::endl;
	for (int i_step = 0; i_step < num_steps; ++i_step)
	{
		std::vector<double> gradient(s);
		const auto cost = problem.CostGradient(current_input_horizon, gradient);

		// normalize gradeint
		const auto norm = std::accumulate(gradient.begin(), gradient.end(), 0,
			[&](double acc, double val)->double {
				return acc + val * val;
			});
		for (int i = 0; i < std::size(gradient); ++i)
		{
			gradient[i] = gradient[i] / norm;
		}

		// add gradient to inputs
		for (int i = 0; i < std::size(gradient); ++i) {
			current_input_horizon[i] = current_input_horizon[i] + gradient[i] * step_rate;
		}

		double current_cost = problem.Cost(current_input_horizon);
		std::cout << "[" << i_step << "]" << " cost=" << current_cost << std::endl;
	}
}
