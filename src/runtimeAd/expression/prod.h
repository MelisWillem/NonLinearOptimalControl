#pragma once

#include<runtimeAd/expression/expression.h>
#include<memory>

namespace runtimeAd {
	class ProdExpression : public BinaryExpression {
	public:
		ProdExpression(const ProdExpression&) = delete;
		ProdExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right
		) : BinaryExpression(left, right) {}

		virtual void forward(
			const std::vector<double>& x)  override
		{
			value = left->value * right->value;
		}

		virtual void backward(std::vector<double>& dx_out)  override
		{
			// f = left * right

			// d(left*right)/d(left) = right
			left->grad += right->value * grad;

			// d(left*right)/d(right) = left
			right->grad += left->value * grad;
		}

		virtual std::string ToString() const override {
			return "(" + left->ToString() + "*" + right->ToString() + ")";
		}
	};
}