#include<runtimeAd/runtimeAd.h>

namespace optimalControl {
	class SystemBehavior {
		using expr = std::vector<std::shared_ptr<runtimeAd::IExpression>>;
	public:
		virtual expr Next(expr& current_state, expr& current_input) = 0;
	};

	class SingleShot {
		using Expr = std::shared_ptr<runtimeAd::IExpression>;
		Expr cost = runtimeAd::Constant(0);
		std::vector<std::shared_ptr<runtimeAd::ConstantValue>> init_state;
		std::vector<std::shared_ptr<runtimeAd::ConstantValue>> ref_state;
		std::vector<std::shared_ptr<runtimeAd::IExpression>> input_horizon;
	public:
		struct InitParams {
			std::shared_ptr<SystemBehavior> system;
			int num_of_input;
			int num_of_states;
			int num_of_steps_horizon;
		};

		SingleShot(InitParams&& params)
		{
			for (int i = 0; i < params.num_of_states; ++i)
			{
				init_state.push_back(runtimeAd::Constant());
				ref_state.push_back(runtimeAd::Constant());
			}

			std::vector<std::shared_ptr<runtimeAd::IExpression>> state;
			for (auto s : init_state)
			{
				state.push_back(std::dynamic_pointer_cast<runtimeAd::IExpression>(s));
			}

			auto cost = std::dynamic_pointer_cast<runtimeAd::IExpression>(runtimeAd::Constant(0));
			for (int i = 0; i < params.num_of_states; i++)
			{
				cost = cost + runtimeAd::Abs(state[i] - ref_state[i]);
			}
			for (int i_step = 0; i_step < params.num_of_steps_horizon; ++i_step)
			{
				std::vector<std::shared_ptr<runtimeAd::IExpression>> inputs(params.num_of_input);
				for (int i = 0; i < params.num_of_input; ++i)
				{
					int start_index_input_horizon = i_step * params.num_of_input;
					inputs[i] = input_horizon[start_index_input_horizon + i];
				}

				state = params.system->Next(state, inputs);
				for (int i = 0; i < params.num_of_states; i++)
				{
					cost = cost + Abs(state[i] - ref_state[i]);
				}
			}

			this->cost = cost;
		}

		double Cost(const std::vector<double>& inputs) { return 0; }
		double CostGradient(
			const std::vector<double>& inputs,
			std::vector<double>& gradient_out) {

			const auto fx = runtimeAd::EvaluateGradient(cost, inputs, gradient_out);

			return fx;
		}
	};
}