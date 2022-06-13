#pragma once

#include<optimalControl/systemBehavior.h>

namespace optimalControl {
	class Simulator {
		DiscreteSystemBehavior& system;
		int num_inputs;
		int num_states;

	public:
		Simulator(DiscreteSystemBehavior& system, int num_inputs, int num_states) :
			system(system),
			num_inputs(num_inputs),
			num_states(num_states)
		{
		}

		std::vector<double> Simulate(
			const std::vector<double>& init_state,
			const std::vector<double>& input_horizon)
		{
			// return end state
			int num_steps = std::size(input_horizon) / num_inputs;
			std::vector<double> state(init_state);

			std::vector<std::shared_ptr<runtimeAd::IExpression>> states_expr;
			for (int i = 0; i < num_states; ++i)
			{
				states_expr.push_back(runtimeAd::Constant());
			}
			std::vector<std::shared_ptr<runtimeAd::IExpression>> inputs_expr;
			for (int i = 0; i < num_inputs; ++i)
			{
				inputs_expr.push_back(runtimeAd::Constant());
			}
			auto new_state_expr = system.Next(states_expr, inputs_expr);

			for (int i_step = 0; i_step < num_steps; ++i_step)
			{
				// set state
				for (int i = 0; i < num_states; ++i)
				{
					states_expr[i]->value = state[i];
				}

				// set input
				const int input_offset = i_step * num_inputs;
				for (int i = 0; i < num_inputs; ++i)
				{
					inputs_expr[i]->value = input_horizon[input_offset + i];
				}

				// simulate
				std::vector<double> x;
				for (int i = 0; i < num_states; ++i)
				{
					state[i] = runtimeAd::EvaluateCost(new_state_expr[i], x);
				}
			}

			return state;
		}
	};
}