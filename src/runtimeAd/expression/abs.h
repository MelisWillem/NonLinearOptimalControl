#pragma once

#include<runtimeAd/expression/expression.h>

namespace runtimeAd {
	class AbsExpression : public UnitaryExpression {
	public:
		AbsExpression(const AbsExpression&) = delete;

		AbsExpression(std::shared_ptr<IExpression> expr) : UnitaryExpression(expr) {}

		virtual void forward(const std::vector<double>& x) override
		{
			value = std::abs(expr->value);
		}

		virtual void backward(std::vector<double>& dx_out)  override
		{
			expr->grad += (expr->value / std::abs(expr->value)) * grad;
		}

		virtual std::string ToString() const override {
			return "|" + expr->ToString() + "|";
		}
	};

	template<
		typename TExpr,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TExpr>>
	>
	inline std::shared_ptr<IExpression> Abs(std::shared_ptr<TExpr> expr)
	{
		return std::make_shared<AbsExpression>(std::dynamic_pointer_cast<IExpression>(expr));
	}
}
