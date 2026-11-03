#include "dpch.h"
#include "Literal.h"
#include "Callable.h"

namespace DXX
{

	std::string LiteralToString(const Literal& literal)
	{
		return std::visit([](const auto& value) -> std::string {

			using T = std::decay_t<decltype(value)>;

			if constexpr (std::is_same_v<T, Nil>) return "NIL";

			else if constexpr (std::is_same_v<T, std::string>) return value;

			else if constexpr (std::is_same_v<T, bool>) return value ? "True" : "False";

			else if constexpr (std::is_same_v<T, Ref<Callable>>) return value ? value->ToString() : "NIL";

			else { std::ostringstream stream; stream << value; return stream.str(); }

			}, literal);
	}

}