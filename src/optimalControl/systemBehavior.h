#pragma once
#include<runtimeAd/runtimeAd.h>

namespace optimalControl {
	class ContiniousSystemBehavior {
	public:
		using multi_expr = std::vector<std::shared_ptr<runtimeAd::IExpression>>;
		virtual multi_expr dot(multi_expr& current_state, multi_expr& current_input) = 0;
	};

	class DiscreteSystemBehavior {
	public:
		using multi_expr = std::vector<std::shared_ptr<runtimeAd::IExpression>>;
		virtual multi_expr Next(multi_expr& current_state, multi_expr& current_input) = 0;
	};
}
