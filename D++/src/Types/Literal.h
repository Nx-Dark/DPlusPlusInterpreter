#pragma once

#include <variant>
#include <string>
#include "Core/Core.h"

namespace DXX
{

	class Callable;
	class ClassInstance;

	struct Nil {
		bool operator==(const Nil&) const = default;
	};

	using Literal = std::variant<Nil, float, double, std::string, bool, Ref<Callable>, Ref<ClassInstance>>;

	std::string LiteralToString(const Literal& literal);

}