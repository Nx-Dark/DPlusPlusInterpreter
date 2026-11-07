#pragma once

#include <variant>
#include <string>
#include "Core/Core.h"

namespace DXX
{

	class Callable;
	class ClassInstance;
	class ListObject;

	struct Nil {
		bool operator==(const Nil&) const = default;
	};

	using Literal = std::variant<Nil, float, double, std::string, bool, Ref<Callable>, Ref<ClassInstance>, Ref<ListObject>>;

	std::string LiteralToString(const Literal& literal);

}