#pragma once
#include<optimalControl/systemBehavior.h>

namespace optimalControl {
	class ExplicitEuler : public DiscreteSystemBehavior {
		optimalControl::ContiniousSystemBehavior& behavior;
		std::shared_ptr<runtimeAd::ConstantValue> step_size;
	public:
		ExplicitEuler(
			optimalControl::ContiniousSystemBehavior& behavior,
			double step_size) : behavior(behavior), step_size(runtimeAd::Constant(step_size))
		{
		}

		virtual multi_expr Next(multi_expr& state, multi_expr& input) override
		{
			auto d_state = behavior.dot(state, input);
			multi_expr out;
			for (int i = 0; i < std::size(state); ++i)
			{
				out.push_back(state[i] + (d_state[i] * step_size));
			}
			return out;
		}
	};

	inline ExplicitEuler IntegrateWithExplicitEuler(ContiniousSystemBehavior& input_system, double step_size)
	{
		return ExplicitEuler(input_system, step_size);
	}
}
