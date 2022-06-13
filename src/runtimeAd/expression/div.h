#pragma once

#include<runtimeAd/expression/expression.h>
#include<memory>

namespace runtimeAd {
	class DivExpression : public BinaryExpression {
	public:
		DivExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right
		) : BinaryExpression(left, right) {}

		virtual void forward(
			const std::vector<double>& x)  override
		{
			value = left->value / right->value;
		}

		virtual void backward(std::vector<double>& dx_out)  override
		{
			// f = g / h
			// df = (g' * h - g * h') / h^2
			const auto denominator = left->value * left->value;

			// d(left*right)/d(left) = right
			left->grad += (right->value / denominator) * grad;

			// d(left*right)/d(right) = left
			right->grad += -(left->value / denominator) * grad;
		}

		virtual std::string ToString() const override {
			return "(" + left->ToString() + "/" + right->ToString() + ")";
		}
	};
}
