#pragma once
#include <cassert>
#include <runtimeAd/expression/expression.h>

namespace runtimeAd {
	class Variable : public IExpression {
		const int location;

	public:
		Variable(const Variable&) = delete;
		Variable(int location) : location(location) {}

		double Value(const std::vector<double>& x) const
		{
			assert(location > -1);
			assert(std::size(x) > location);
			return x[location];
		}

		virtual void forward(const std::vector<double>& x) override
		{
			value = Value(x);
		}

		virtual void backward(std::vector<double>& dx_out) override
		{
			// write away the value to the actual output vector
			// add to it, as it may contain multiple terms
			dx_out[location] += grad;
			// reset grad so the same stuff doesn't get added twice
			// -> VisitEveryTime() = True here
			grad = 0;
		}

		// Every variable must be visited multiple times, as
		// there is no garantue that it has the same instance.
		// It functions like a value type and not a reference type.
		virtual bool VisitEveryTime() const override { return true; }

		virtual std::string ToString() const override {
			return "x[" + std::to_string(location) + "]";
		}
	};

	inline std::shared_ptr<Variable> Var(int location)
	{
		return std::make_shared<Variable>(location);
	}
}
