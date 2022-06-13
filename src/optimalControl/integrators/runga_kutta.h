#pragma once

#include<optimalControl/systemBehavior.h>

namespace optimalControl {
	// 	def integrator_RK(x, step_size, function_system) :
	// 	"""
	// 	Integrate with Runga Kutta
	// 	Parameters
	// 	x : state
	// 	step_size
	// 	function_system
	// 	"""
	// 	k1 = function_system(x)
	// 	k2 = function_system(x + step_size * k1 / 2)
	// 	k3 = function_system(x + step_size * k2 / 2)
	// 	k4 = function_system(x + step_size * k3)
	// 	x_new = x + (step_size / 6) * (k1 + 2 * k2 + 2 * k3 + k4)
	// 
	// 	return  x_new
	class RungaKutta : public DiscreteSystemBehavior {
		optimalControl::ContiniousSystemBehavior& behavior;
		std::shared_ptr<runtimeAd::ConstantValue> step_size;
	public:
		RungaKutta(
			optimalControl::ContiniousSystemBehavior& behavior,
			double step_size) : behavior(behavior), step_size(runtimeAd::Constant(step_size))
		{
		}

		virtual multi_expr Next(multi_expr& state, multi_expr& input) override
		{
			// 	k1 = function_system(x)
			multi_expr k1 = behavior.dot(state, input);

			// 	k2 = function_system(x + step_size * k1 / 2)
			multi_expr state_k2;
			for (int i = 0; i < std::size(state); ++i)
			{
				state_k2.push_back(state[i] + (step_size * k1[i]) / runtimeAd::Var(2));
			}
			auto k2 = behavior.dot(state_k2, input);

			// 	k3 = function_system(x + step_size * k2 / 2)
			multi_expr state_k3;
			for (int i = 0; i < std::size(state); ++i)
			{
				state_k3.push_back(state[i] + (step_size * k2[i]) / runtimeAd::Var(2));
			}
			auto k3 = behavior.dot(state_k3, input);

			// 	k4 = function_system(x + step_size * k3)
			multi_expr state_k4;
			for (int i = 0; i < std::size(state); ++i)
			{
				state_k4.push_back(state[i] + (step_size * k3[i]));
			}
			auto k4 = behavior.dot(state_k4, input);

			multi_expr out;
			// 	x_new = x + (step_size / 6) * (k1 + 2 * k2 + 2 * k3 + k4)
			auto constant_6 = runtimeAd::Constant(6);
			auto constant_2 = runtimeAd::Constant(2);

			for (int i = 0; i < std::size(state); i++)
			{
				runtimeAd::Expr new_s = state[i] + (step_size / constant_6) * (k1[i] + constant_2 * k2[i] + constant_2 * k3[i] + k4[i]);
				out.push_back(new_s);
			}

			return out;
		}
	};

	inline RungaKutta IntegrateWithRungaKutta(ContiniousSystemBehavior& system, double step_size)
	{
		return RungaKutta(system, step_size);
	}
}