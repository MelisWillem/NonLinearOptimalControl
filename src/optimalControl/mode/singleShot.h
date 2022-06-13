#include<runtimeAd/runtimeAd.h>
#include <optimalControl/systemBehavior.h>

namespace optimalControl {
	class SingleShot {
		using Expr = std::shared_ptr<runtimeAd::IExpression>;
		Expr cost = runtimeAd::Constant(0);
		std::vector<std::shared_ptr<runtimeAd::ConstantValue>> init_state;
		std::vector<std::shared_ptr<runtimeAd::ConstantValue>> ref_state;
		std::vector<std::shared_ptr<runtimeAd::IExpression>> input_horizon;

		int gradient_size;
	public:
		int GradientSize() const { return gradient_size; }

		void SetRefState(std::vector<double> ref_state) {
			assert(std::size(ref_state) == std::size(this->ref_state));
			for (int i = 0; i < std::size(ref_state); ++i)
			{
				this->ref_state[i]->value = ref_state[i];
			}
		}

		void SetInitState(std::vector<double> init_state) {
			assert(std::size(init_state) == std::size(this->init_state));
			for (int i = 0; i < std::size(init_state); ++i)
			{
				this->init_state[i]->value = init_state[i];
			}
		}

		struct InitParams {
			DiscreteSystemBehavior* system;
			int num_of_input;
			int num_of_states;
			int num_of_steps_horizon;
		};

		SingleShot(InitParams& params)
		{
			gradient_size = params.num_of_input * params.num_of_steps_horizon;

			for (int i = 0; i < params.num_of_states; ++i)
			{
				init_state.push_back(runtimeAd::Constant());
				ref_state.push_back(runtimeAd::Constant());
			}

			// push the initial state
			std::vector<std::shared_ptr<runtimeAd::IExpression>> state;
			for (auto s : init_state)
			{
				state.push_back(std::dynamic_pointer_cast<runtimeAd::IExpression>(s));
			}

			// start with a cost of ozero
			auto cost = std::dynamic_pointer_cast<runtimeAd::IExpression>(runtimeAd::Constant(0));
			// set the cost of the initial state
			for (int i = 0; i < params.num_of_states; i++)
			{
				cost = cost + runtimeAd::Abs(state[i] - ref_state[i]);
			}

			// walk over the horizon
			for (int i_step = 0; i_step < params.num_of_steps_horizon; ++i_step)
			{
				// Put the inputs of this step into expression form.
				std::vector<std::shared_ptr<runtimeAd::IExpression>> inputs(params.num_of_input);
				for (int i = 0; i < params.num_of_input; ++i)
				{
					const int start_index_input_horizon = i_step * params.num_of_input;
					inputs[i] = input_horizon[start_index_input_horizon + i];
				}

				state = params.system->Next(state, inputs);
				// Evaluate the constraint on the new state.
				for (int i = 0; i < params.num_of_states; i++)
				{
					cost = cost + Abs(state[i] - ref_state[i]);
				}
			}

			this->cost = cost;
		}

		double Cost(const std::vector<double>& inputs) { return  runtimeAd::EvaluateCost(cost, inputs); }
		double CostGradient(
			const std::vector<double>& inputs,
			std::vector<double>& gradient_out) {
			assert(std::size(inputs) == gradient_size);
			assert(std::size(gradient_out) == gradient_size);

			const auto fx = runtimeAd::EvaluateGradient(cost, inputs, gradient_out);

			return fx;
		}
	};
}