#pragma once

#include <variant>
#include <string>

namespace DXX
{

	struct Nil {
		bool operator==(const Nil&) const = default;
	};

	using Literal = std::variant<Nil, float, std::string, bool>;

	inline std::string LiteralToString(const Literal& literal)
	{
		return std::visit([](const auto& value) -> std::string {

			using T = std::decay_t<decltype(value)>;

			if constexpr (std::is_same_v<T, Nil>) return "NIL";

			else if constexpr (std::is_same_v<T, std::string>) return value;

			else if constexpr (std::is_same_v<T, bool>) return value ? "True" : "False";

			else { std::ostringstream stream; stream << value; return stream.str(); }

		}, literal);
	}
}