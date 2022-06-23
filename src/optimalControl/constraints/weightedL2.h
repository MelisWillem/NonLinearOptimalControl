#pragma once
#include<runtimeAd/runtimeAd.h>

namespace optimalControl {
	class WeightedL2Constraint {
		std::vector<runtimeAd::Expr> weights;

	public:
		WeightedL2Constraint() = default;

		WeightedL2Constraint(const std::vector<double>& weights)
		{
			for (auto v : weights)
			{
				this->weights.push_back(std::dynamic_pointer_cast<runtimeAd::IExpression>(runtimeAd::Constant(v)));
			}
		}

		template<
			typename TExprRef,
			typename = std::enable_if_t<std::is_base_of_v<runtimeAd::IExpression, TExprRef>>
		>
		runtimeAd::Expr Eval(const std::vector<runtimeAd::Expr>& x, const std::vector<std::shared_ptr<TExprRef>>& x_ref) {
			runtimeAd::Expr out = std::dynamic_pointer_cast<runtimeAd::IExpression>(runtimeAd::Constant(0));
			if (std::empty(weights))
			{
				// No weights intialized, there is no state constraint.
				return out;
			}

			assert(std::size(x) == std::size(x_ref));
			assert(std::size(x) == std::size(weights));

			for (int i = 0; i < std::size(x); ++i)
			{
				auto diff = x[i] - x_ref[i];
				out = out + (weights[i] * diff * diff);
			}

			return out;
		}
	};
}
