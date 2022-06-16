#pragma once

#include<runtimeAd/expression/expression.h>

namespace runtimeAd {
	class ConstantValue : public IExpression
	{
	public:
		virtual void ZeroGrad() override {}

		virtual bool VisitRequired() const override { return false; }

		ConstantValue() = delete;

		ConstantValue(double value)
		{
			this->value = value;
		}

		virtual void forward(const std::vector<double>& x) override
		{
			// nothing to do there
		}

		virtual void backward(std::vector<double>& dx_out) override
		{
			// constants don't change the gradient
			// do nothing
		}

		virtual std::string ToString() const override {
			return "C(" + std::to_string(value) + ")";
		}
	};

	inline std::shared_ptr<ConstantValue> Constant(double value = 0)
	{
		return std::make_shared<ConstantValue>(value);
	}
}