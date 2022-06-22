#pragma once

#include<runtimeAd/expression/expression.h>
#include<cmath>

namespace runtimeAd {
	class CosExpression : public UnitaryExpression {
	public:
		CosExpression(const CosExpression&) = delete;

		CosExpression(std::shared_ptr<IExpression> expr) : UnitaryExpression(expr) {}

		virtual void forward(const std::vector<double>& x) override
		{
			value = std::cos(expr->value);
		}

		virtual void backward(std::vector<double>& dx_out)  override
		{
			expr->grad += -std::sin(expr->value) * grad;
		}

		virtual std::string ToString() const override {
			return "cos(" + expr->ToString() + ")";
		}
	};

	template<
		typename TExpr,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TExpr>>
	>
	inline std::shared_ptr<IExpression> cos(
		const std::shared_ptr<TExpr> expr)
	{
		return std::make_shared<CosExpression>(
			std::dynamic_pointer_cast<IExpression>(expr));
	}

	class SinExpression : public UnitaryExpression {
	public:
		SinExpression(const SinExpression&) = delete;

		SinExpression(std::shared_ptr<IExpression> expr) : UnitaryExpression(expr) {}

		virtual void forward(
			const std::vector<double>& x) override
		{
			value = std::sin(expr->value);
		}

		virtual void backward(std::vector<double>& dx_out) override
		{
			const auto f = expr->value;
			const auto cos_f = std::cos(f);
			expr->grad += cos_f * grad;
		}

		virtual std::string ToString() const override {
			return "sin(" + expr->ToString() + ")";
		}
	};

	template<
		typename TExpr,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TExpr>>
	>
	inline std::shared_ptr<IExpression> sin(
		const std::shared_ptr<TExpr> expr)
	{
		return std::make_shared<SinExpression>(
			std::dynamic_pointer_cast<IExpression>(expr));
	}
}
