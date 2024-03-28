#pragma once

#include<vector>
#include<stack> 
#include<set>
#include<memory>
#include<string>

namespace runtimeAd {

	class Function;

	class IExpression {
		auto CreateVisitTree(IExpression* topNode)
		{
			std::vector<IExpression*> to_visit;
			std::set<IExpression*> nodes_visited;

			topNode->AddChildren(to_visit, nodes_visited);
			to_visit.push_back(topNode);
			return to_visit;
		}
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
			std::vector<IExpression*>& visits,
			std::set<IExpression*>& nodes_already_visited) const {}

		virtual std::string ToString() const = 0;

		virtual bool VisitEveryTime() const { return false; }
		virtual bool GradientRequired() const { return true; }

		std::unique_ptr<Function> Compile();
	};

	template<typename TExpr>
	inline bool NeedsVisit(std::set<IExpression*>& already_visited, const std::shared_ptr<TExpr>& node)
	{
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

		virtual void ZeroGrad() override
		{
			grad = 0;
			value = 0;
			left->ZeroGrad();
			right->ZeroGrad();
		}

		virtual void AddChildren(
			std::vector<IExpression*>& visits,
			std::set<IExpression*>& nodes_already_visited) const override {
			// depth first
			if (left->GradientRequired() && NeedsVisit(nodes_already_visited, left))
			{
				left->AddChildren(visits, nodes_already_visited);
				visits.push_back(left.get());
				nodes_already_visited.insert(left.get());
			}
			if (right->GradientRequired() && NeedsVisit(nodes_already_visited, right))
			{
				right->AddChildren(visits, nodes_already_visited);
				visits.push_back(right.get());
				nodes_already_visited.insert(right.get());
			}
		}
	};

	class UnitaryExpression : public IExpression {
	public:
		std::shared_ptr<IExpression> expr;

		UnitaryExpression(std::shared_ptr<IExpression> expr) : expr(expr) {}

		virtual void ZeroGrad() override
		{
			grad = 0;
			value = 0;
			expr->ZeroGrad();
		}

		virtual void AddChildren(
			std::vector<IExpression*>& visits,
			std::set<IExpression*>& nodes_already_visited) const override {
			if (expr->GradientRequired() && NeedsVisit(nodes_already_visited, expr)) {
				expr->AddChildren(visits, nodes_already_visited);
				visits.push_back(expr.get());
				nodes_already_visited.insert(expr.get());
			}
		}
	};
}
