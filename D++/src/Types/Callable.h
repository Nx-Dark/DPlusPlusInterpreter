#pragma once

#include <vector>
#include "Literal.h"

namespace DXX
{

	class Interpreter;

	class Callable
	{
	public:
		virtual ~Callable() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) = 0;

		virtual uint32_t arity() const = 0;

		virtual std::string ToString() const = 0;
	};

}