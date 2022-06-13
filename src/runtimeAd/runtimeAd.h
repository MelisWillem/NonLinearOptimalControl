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
		// -1 means no parent, not a problem as this is the seed element
		to_visit.push_back(topNode);
		topNode->AddChildren(to_visit);
		return to_visit;
	}

	inline double EvaluateCost(
		std::shared_ptr<IExpression> root,
		const std::vector<double>& x)
	{
		const auto to_visit = CreateVisitTree(root);

		// evaluate the function -> forward pass
		for (int i = std::size(to_visit) - 1; i > -1; --i)
		{
			to_visit[i]->forward(x);
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

		// evaluate the function -> forward pass
		for (int i = std::size(to_visit) - 1; i > -1; --i)
		{
			to_visit[i]->forward(x);
		}
		const double fx = root->value;

		root->ZeroGrad();
		root->grad = 1; // set the seed
		// evaluate the gradient -> backward pass
		for (int i = 0; i < std::size(to_visit); ++i)
		{
			to_visit[i]->backward(dx);
		}

		return fx;
	}
}
