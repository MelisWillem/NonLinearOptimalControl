#pragma once

#include<runtimeAd/expression/expression.h>
#include<memory>

namespace runtimeAd {

	class SumExpression : public BinaryExpression {
	public:
		SumExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right
		) : BinaryExpression(left, right) {}

		virtual void forward(
			const std::vector<double>& x
			) override
		{
			value = left->value + right->value;
		}

		virtual void backward(std::vector<double>& dx_out) override
		{
			// f = left + right

			// df/dleft = 1
			left->grad += grad * 1;

			// df/dright = 1
			right->grad += grad * 1;
		}

		virtual std::string ToString() const override {
			return "(" + left->ToString() + "+" + right->ToString() + ")";
		}
	};
}