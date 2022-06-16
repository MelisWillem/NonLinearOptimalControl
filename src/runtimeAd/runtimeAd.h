#pragma once

#include<runtimeAd/expression/expression.h>
#include<runtimeAd/expression/constant.h>
#include<runtimeAd/expression/variable.h>
#include<runtimeAd/expression/sum.h>
#include<runtimeAd/expression/sub.h>
#include<runtimeAd/expression/abs.h>
#include<runtimeAd/expression/operators.h>
#include<runtimeAd/expression/geometric.h>

namespace runtimeAd {
	using Expr = std::shared_ptr<IExpression>;

	inline auto CreateVisitTree(const std::shared_ptr<IExpression>& topNode)
	{
		std::vector<std::shared_ptr<IExpression>> to_visit;
		std::set<IExpression*> nodes_visited;

		topNode->AddChildren(to_visit, nodes_visited);
		to_visit.push_back(topNode);
		return to_visit;
	}

	inline double EvaluateCost(
		std::shared_ptr<IExpression> root,
		const std::vector<double>& x)
	{
		const auto to_visit = CreateVisitTree(root);

		// evaluate the function -> forward pass
		for (int i = 0; i < std::size(to_visit); ++i)
		{
			to_visit[i]->forward(x);
			auto str_expr = to_visit[i]->ToString();
		}
		const double fx = root->value;

		return fx;
	}

	/// <summary>
	/// Evaluate gradient and function -> dx should be of the right size
	/// </summary>
	/// <param name="root">top value of the function</param>
	/// <param name="x"></param>
	/// <param name="dx">gradient is written in this</param>
	/// <returns>function evaluation</returns>
	inline double EvaluateGradient(
		std::shared_ptr<IExpression> root,
		const std::vector<double>& x,
		std::vector<double>& dx)
	{
		const auto to_visit = CreateVisitTree(root);
		root->ZeroGrad();

		// evaluate the function -> forward pass
		for (int i = 0; i < std::size(to_visit); ++i)
		{
			auto str_expr = to_visit[i]->ToString();
			to_visit[i]->forward(x);
		}
		const double fx = root->value;

		root->grad = 1; // set the seed
		// evaluate the gradient -> backward pass
		for (int i = std::size(to_visit) - 1; i > -1; --i)
		{
			auto str_expr = to_visit[i]->ToString();
			to_visit[i]->backward(dx);
		}

		return fx;
	}

	inline double EvaluateGradientFiniteDifference(
		std::shared_ptr<IExpression> root,
		const std::vector<double>& x,
		std::vector<double>& dx)
	{
		const double f_eval = EvaluateCost(root, x);
		auto f_str = root->ToString();

		const double eps = 1e-6;
		std::vector<double> diff(std::size(x));
		for (int i_diff = 0; i_diff < std::size(x); i_diff++)
		{
			for (int i = 0; i < std::size(x); ++i)
			{
				diff[i] = x[i];
			}
			diff[i_diff] = diff[i_diff] + eps;
			root->ZeroGrad();
			const double f_eval_eps = EvaluateCost(root, diff);

			dx[i_diff] = (f_eval_eps - f_eval) / eps;
		}

		return f_eval;
	}
}
