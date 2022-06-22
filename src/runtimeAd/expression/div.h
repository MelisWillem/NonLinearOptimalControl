#pragma once

#include<runtimeAd/expression/expression.h>
#include<memory>

namespace runtimeAd {
	class DivExpression : public BinaryExpression {
	public:
		DivExpression(const DivExpression&) = delete;

		DivExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right
		) : BinaryExpression(left, right) {}

		virtual void forward(
			const std::vector<double>& x)  override
		{
			value = left->value / right->value;
			assert(!isnan(value));
		}

		virtual void backward(std::vector<double>& dx_out)  override
		{
			// d(left/right)/d(left) = 1/right
			left->grad += (1 / right->value) * grad;

			// d(left/right)/d(right) 
			// = d(left*right^{-1})/d(right) 
			// = -left*(right^{-2}) 
			// = -left/right^2
			right->grad += -(left->value / (right->value * right->value)) * grad;
		}

		virtual std::string ToString() const override {
			return "(" + left->ToString() + "/" + right->ToString() + ")";
		}
	};
}
