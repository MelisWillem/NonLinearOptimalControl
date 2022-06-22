#pragma once

#include<vector>
#include<stack> 
#include<set>
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
			value = 0;
		}

		virtual ~IExpression() = default;

		virtual void forward(const std::vector<double>& x) = 0;
		virtual void backward(std::vector<double>& dx_out) = 0;

		virtual void AddChildren(
			std::vector<std::shared_ptr<IExpression>>& visits,
			std::set<IExpression*>& nodes_already_visited) const {}

		virtual std::string ToString() const = 0;

		virtual bool VisitEveryTime() const { return false; }
		virtual bool VisitRequired() const { return true; }
		virtual bool Skip() const { return false; }
	};

	template<typename TExpr>
	inline bool NeedsVisit(std::set<IExpression*>& already_visited, const std::shared_ptr<TExpr>& node)
	{
		if (node->Skip())
		{
			// we can skip it
			return false;
		}

		IExpression* expr = node.get();
		const bool node_is_in_set = already_visited.find(expr) != already_visited.end();
		if (node_is_in_set)
		{
			// Node is in the set, already visit if you need to visit all.
			bool nv = node->VisitEveryTime();
			return nv;
		}

		// node is not in the set -> visit
		return true;
	}

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
			value = 0;
			left->ZeroGrad();
			right->ZeroGrad();
		}

		virtual void AddChildren(
			std::vector<std::shared_ptr<IExpression>>& visits,
			std::set<IExpression*>& nodes_already_visited) const override {
			auto left_str = left->ToString();
			auto right_str = right->ToString();

			// depth first
			if (left->VisitRequired() && NeedsVisit(nodes_already_visited, left))
			{
				left->AddChildren(visits, nodes_already_visited);
				visits.push_back(left);
				nodes_already_visited.insert(left.get());
			}
			if (right->VisitRequired() && NeedsVisit(nodes_already_visited, right))
			{
				right->AddChildren(visits, nodes_already_visited);
				visits.push_back(right);
				nodes_already_visited.insert(right.get());
			}
		}
	};

	class UnitaryExpression : public IExpression {
	public:
		std::shared_ptr<IExpression> expr;

		UnitaryExpression(std::shared_ptr<IExpression> expr) : expr(expr) {}

		virtual void ZeroGrad()
		{
			grad = 0;
			value = 0;
			expr->ZeroGrad();
		}

		virtual void AddChildren(
			std::vector<std::shared_ptr<IExpression>>& visits,
			std::set<IExpression*>& nodes_already_visited) const override {
			if (expr->VisitRequired() && NeedsVisit(nodes_already_visited, expr)) {
				expr->AddChildren(visits, nodes_already_visited);
				visits.push_back(expr);
				nodes_already_visited.insert(expr.get());
			}
		}
	};
}
