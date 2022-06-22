#pragma once

#include<runtimeAd/expression/expression.h>

namespace runtimeAd {
	class SubExpression : public BinaryExpression {
	public:
		SubExpression(const SubExpression&) = delete;
		SubExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right
		) : BinaryExpression(left, right) {}

		virtual void forward(
			const std::vector<double>& x
		) override
		{
			value = left->value - right->value;
		}

		virtual void backward(std::vector<double>& dx_out) override
		{
			// f = left + right

			// df/dleft = 1
			left->grad += grad * 1;

			// df/dright = 1
			right->grad += -grad * 1;
		}

		virtual std::string ToString() const override {
			return "(" + left->ToString() + "-" + right->ToString() + ")";
		}
	};
}