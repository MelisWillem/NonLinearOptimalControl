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
		auto constant_m = runtimeAd::Constant(m);

		multi_expr next_state;
		next_state.push_back(current_state[2]);
		next_state.push_back(current_state[3]);
		next_state.push_back(current_input[0] / constant_m);
		next_state.push_back(current_input[1] / constant_m);

		return next_state;
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
	params.num_of_steps_horizon = 10;
	params.system = &discrete_system;

	optimalControl::SingleShot problem(params);
	auto s = problem.GradientSize();

	// Start at origin at stand still.
	std::vector<double> init_state = { 0,0, 0,0 };
	problem.SetInitState(init_state);

	// End at (10, 10) at standing still.
	std::vector<double> ref_state = { 10, 10 , 0, 0 };
	problem.SetRefState(ref_state);

	std::vector<double> current_input_horizon(s);
	for (int i = 0; i < std::size(current_input_horizon); ++i)
	{
		current_input_horizon[i] = 0;
	}

	int num_steps = 10;
	double cost = problem.Cost(current_input_horizon);
	// double step_rate = 1e3;
	std::cout << "Starting with a cost = " << cost << std::endl;
	for (int i_step = 0; i_step < num_steps; ++i_step)
	{
		std::vector<double> gradient(s);
		problem.CostGradient(current_input_horizon, gradient);

		std::vector<double> gradientFiniteDifference(s);
		problem.CostGradientFiniteDifference(current_input_horizon, gradientFiniteDifference);

		// add gradient to inputs
		for (int i = 0; i < std::size(gradient); ++i) {
			current_input_horizon[i] = current_input_horizon[i] - gradient[i];
		}

		// clip the input horizon
		const double max_input = 1;
		const double min_input = -1;
		for (int i = 0; i < std::size(gradient); ++i) {
			current_input_horizon[i] = std::max(current_input_horizon[i], min_input);
			current_input_horizon[i] = std::min(current_input_horizon[i], max_input);
		}

		const double new_cost = problem.Cost(current_input_horizon);
		const double res = cost - new_cost;
		if (i_step % 1 == 0)
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

}
