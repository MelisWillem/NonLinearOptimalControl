#pragma once

#include<runtimeAd/expression/prod.h>
#include<runtimeAd/expression/sum.h>
#include<memory>

namespace runtimeAd {

	template<
		typename TLeft,
		typename TRight,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TLeft>>,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TRight>>
	>
	inline std::shared_ptr<IExpression> operator+(
		const std::shared_ptr<TLeft> left,
		const std::shared_ptr<TRight> right)
	{
		return std::make_shared<SumExpression>(
			std::dynamic_pointer_cast<IExpression>(left),
			std::dynamic_pointer_cast<IExpression>(right));
	}

	template<
		typename TLeft,
		typename TRight,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TLeft>>,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TRight>>
	>
	inline std::shared_ptr<IExpression> operator-(
		const std::shared_ptr<TLeft> left,
		const std::shared_ptr<TRight> right)
	{
		return std::make_shared<SubExpression>(
			std::dynamic_pointer_cast<IExpression>(left),
			std::dynamic_pointer_cast<IExpression>(right));
	}

	template<
		typename TLeft,
		typename TRight,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TLeft>>,
		typename = std::enable_if_t<std::is_base_of_v<IExpression, TRight>>
	>
	inline std::shared_ptr<IExpression> operator*(
		const std::shared_ptr<TLeft> left,
		const std::shared_ptr<TRight> right)
	{
		return std::make_shared<ProdExpression>(
			std::dynamic_pointer_cast<IExpression>(left),
			std::dynamic_pointer_cast<IExpression>(right));
	}
}
