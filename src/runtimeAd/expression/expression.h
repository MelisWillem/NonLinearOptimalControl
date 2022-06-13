#pragma once

#include<vector>
#include<stack>
#include<memory>
#include<string>

namespace runtimeAd {
	class IExpression {
	public:
		double value = 0;
		double grad = 0;

		virtual void ZeroGrad()
		{
			grad = 0;
		}

		virtual ~IExpression() = default;

		virtual void forward(const std::vector<double>& x) = 0;
		virtual void backward(std::vector<double>& dx_out) = 0;

		virtual void AddChildren(std::vector<std::shared_ptr<IExpression>>& visits) const {}

		virtual std::string ToString() const = 0;
	};

	class BinaryExpression : public IExpression {
	public:
		std::shared_ptr<IExpression> left;
		std::shared_ptr<IExpression> right;

		BinaryExpression(
			std::shared_ptr<IExpression> left,
			std::shared_ptr<IExpression> right) :
			left(left), right(right) {}

		virtual void ZeroGrad()
		{
			grad = 0;
			left->ZeroGrad();
			right->ZeroGrad();
		}

		virtual void AddChildren(std::vector<std::shared_ptr<IExpression>>& visits) const override {
			const int current_location = std::size(visits) - 1;
			visits.push_back(left);
			visits.push_back(right);
			left->AddChildren(visits);
			right->AddChildren(visits);
		}
	};

	class UnitaryExpression : public IExpression {
	public:
		std::shared_ptr<IExpression> expr;

		UnitaryExpression(std::shared_ptr<IExpression> expr) : expr(expr) {}

		virtual void ZeroGrad()
		{
			grad = 0;
			expr->ZeroGrad();
		}

		virtual void AddChildren(std::vector<std::shared_ptr<IExpression>>& visits) const override {
			visits.push_back(expr);
			expr->AddChildren(visits);
		}
	};
}