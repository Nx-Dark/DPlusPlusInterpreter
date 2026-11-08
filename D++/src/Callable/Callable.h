#pragma once

#include <vector>
#include "Types/Literal.h"

namespace DXX
{

	static constexpr uint32_t VARIADIC_ARITY{ UINT32_MAX };

	class Interpreter;

	class Callable
	{
	public:
		virtual ~Callable() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) = 0;

		virtual uint32_t minArity() const = 0;
		virtual uint32_t maxArity() const = 0;

		virtual std::string ToString() const = 0;
	};

}