#include<optimalControl/optimalControl.h>
#include<catch2/catch_test_macros.hpp>
#include<catch2/catch_approx.hpp>
#include<numeric>
#include<iostream>

// # Symbols / expressions
constexpr int number_of_states = 4;
constexpr int number_of_inputs = 2;

// The system behavior :
// dot_x0 = x_2
// dot_x1 = x_3
// dot_x2 = u_0 / m
// dot_x3 = u_1 / m
class CartBehavior : public optimalControl::ContiniousSystemBehavior
{
	virtual multi_expr dot(multi_expr& current_state, multi_expr& current_input) override {
		using namespace runtimeAd;
		const double m = 1;
		auto constant_m = runtimeAd::Constant(m);

		multi_expr next_state;
		next_state.push_back(current_state[2]);
		next_state.push_back(current_state[3]);
		next_state.push_back(current_input[0] / constant_m);
		next_state.push_back(current_input[1] / constant_m);

		return next_state;
	}
};

void Normalize(std::vector<double>& x)
{
	double norm2 = 0;
	for (int i = 0; i < std::size(x); ++i)
	{
		norm2 = norm2 + (x[i] * x[i]);
	}
	norm2 = std::sqrt(norm2);


	for (int i = 0; i < std::size(x); ++i)
	{
		x[i] = x[i] / norm2;
	}
}

TEST_CASE("Give_Simpl_Cart_System_Move_To_Reference_Single_Shot")
{
	CartBehavior cont_system;

	double step_size = 1e-1;
	auto discrete_system = optimalControl::IntegrateWithExplicitEuler(cont_system, step_size);

	optimalControl::SingleShot::InitParams params;
	params.num_of_input = 2; // 2D motoro speed
	params.num_of_states = 4; // 2D position + speed of body
	params.num_of_steps_horizon = 10;
	params.system = &discrete_system;
	optimalControl::WeightedL2Constraint state_constraint({ 10.0, 10.0, 0.0, 0.0 });
	params.state_constraint = state_constraint;

	optimalControl::SingleShot problem(params);
	auto s = problem.GradientSize();

	// Start at origin at stand still.
	std::vector<double> init_state = { 0,0, 0,0 };
	problem.SetInitState(init_state);

	// End at (10, 10) at standing still.
	std::vector<double> ref_state = { 10, 10 , 5, 5 };
	problem.SetRefState(ref_state);

	std::vector<double> current_input_horizon(s);
	for (int i = 0; i < std::size(current_input_horizon); ++i)
	{
		current_input_horizon[i] = 0;
	}

	int num_steps = 1000;
	double cost = problem.Cost(current_input_horizon);
	double step_size_gradient = 1e-1;
	std::cout << "Starting with a cost = " << cost << std::endl;
	for (int i_step = 0; i_step < num_steps; ++i_step)
	{
		std::vector<double> gradient(s);
		const double current_cost = problem.CostGradient(current_input_horizon, gradient);

		// Test if the gradient is about the same as the finite difference once.
		std::vector<double> gradientFiniteDifference(s);
		problem.CostGradientFiniteDifference(current_input_horizon, gradientFiniteDifference);
		for (int i = 0; i < std::size(gradientFiniteDifference); ++i)
		{
			auto diff = std::abs(gradient[i] - gradientFiniteDifference[i]);
			REQUIRE(diff < 1e-3);
		}

		// Normalize(gradient);

		// add gradient to inputs
		for (int i = 0; i < std::size(gradient); ++i) {
			current_input_horizon[i] = current_input_horizon[i] - step_size_gradient * gradient[i];
		}

		// clip the input horizon
		const double max_input = 30;
		const double min_input = -30;
		for (int i = 0; i < std::size(gradient); ++i) {
			current_input_horizon[i] = std::max(current_input_horizon[i], min_input);
			current_input_horizon[i] = std::min(current_input_horizon[i], max_input);
		}

		const double new_cost = problem.Cost(current_input_horizon);
		const double res = cost - new_cost;
		if (i_step % 10 == 0)
		{
			std::cout << "[" << i_step << "]" << " cost=" << new_cost << "  residual=" << res << std::endl;
		}
		cost = new_cost;
	}

	// apply the inputs to a simulator of the system
	std::cout << "Simulating with the input horizon ..." << std::endl;
	optimalControl::Simulator sim(discrete_system, number_of_inputs, number_of_states);
	auto final_state = sim.Simulate(init_state, current_input_horizon);

	std::cout << "final state: [";
	for (int i = 0; i < std::size(final_state); ++i) {
		std::cout << final_state[i] << "; ";
	}
	std::cout << "]" << std::endl;

	for (int i = 0; i < std::size(current_input_horizon) / 2; i++)
	{
		std::cout << "{" << current_input_horizon[i * 2] << "; " << current_input_horizon[i * 2 + 1] << "}";
	}

}
