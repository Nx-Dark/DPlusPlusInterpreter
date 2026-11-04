#pragma once

#include <variant>
#include <string>
#include "Core/Core.h"

namespace DXX
{

	class Callable;

	struct Nil {
		bool operator==(const Nil&) const = default;
	};

	using Literal = std::variant<Nil, float, uint64_t, std::string, bool, Ref<Callable>>;

	std::string LiteralToString(const Literal& literal);

}