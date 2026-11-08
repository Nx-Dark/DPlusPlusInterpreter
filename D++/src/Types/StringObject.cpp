#include "dpch.h"
#include "StringObject.h"

#include "Interpreter/Interpreter.h"

namespace DXX
{

	static std::size_t validateIndex(Interpreter& interpreter, const Literal& indexLiteral)
	{
		double index{};

		if (auto indexPtr{ std::get_if<double>(&indexLiteral) })
			index = *indexPtr;
		else
			throw interpreter.nativeError("Index must be a Number.");

		if (!std::isfinite(index))
			throw interpreter.nativeError("Index must be finite.");

		if (std::floor(index) != index)
			throw interpreter.nativeError("Index must be an Integer.");

		if (index < 0.0f)
			throw interpreter.nativeError("Index cannot be negative.");

		return static_cast<std::size_t>(index);
	}

	Literal StringLength::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::size_t length{ m_String.size() };
		return static_cast<double>(length);
	}
	
	Literal StringUpper::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::string string{ m_String };
		std::transform(string.begin(), string.end(), string.begin(), [](unsigned char ch) {
			return std::toupper(ch);
		});
		return string;
	}

	Literal StringLower::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::string string{ m_String };
		std::transform(string.begin(), string.end(), string.begin(), [](unsigned char ch) {
			return std::tolower(ch);
		});
		return string;
	}

	Literal StringContains::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& subStrLiteral{ arguments[0] };

		if (!(std::holds_alternative<std::string>(subStrLiteral)))
			throw interpreter.nativeError("contains requires a string argument.");

		const std::string_view subStr{ std::get<std::string>(subStrLiteral) };
		return m_String.contains(subStr);
	}

	Literal StringStartsWith::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& subStrLiteral{ arguments[0] };

		if (!(std::holds_alternative<std::string>(subStrLiteral)))
			throw interpreter.nativeError("startsWith requires a string argument.");

		const std::string_view subStr{ std::get<std::string>(subStrLiteral) };
		return m_String.starts_with(subStr);
	}

	Literal StringEndsWith::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& subStrLiteral{ arguments[0] };

		if (!(std::holds_alternative<std::string>(subStrLiteral)))
			throw interpreter.nativeError("startsWith requires a string argument.");

		const std::string_view subStr{ std::get<std::string>(subStrLiteral) };
		return m_String.ends_with(subStr);
	}

	Literal StringIndexOf::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& subStrLiteral{ arguments[0] };

		if (!(std::holds_alternative<std::string>(subStrLiteral)))
			throw interpreter.nativeError("IndexOf requires a string argument.");

		const std::string_view subStr{ std::get<std::string>(subStrLiteral) };

		std::size_t index{ m_String.find(subStr) };

		if (index == std::string::npos || subStr.empty())
			return -1.0;

		return static_cast<double>(index);
	}

	Literal StringReplace::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& oldStrLiteral{ arguments[0] };
		const Literal& newStrLiteral{ arguments[1] };

		if (!(std::holds_alternative<std::string>(oldStrLiteral)) || !(std::holds_alternative<std::string>(newStrLiteral)))
			throw interpreter.nativeError("IndexOf requires a string argument.");

		const std::string_view oldStr{ std::get<std::string>(oldStrLiteral) };
		const std::string_view newStr{ std::get<std::string>(newStrLiteral) };

		std::string string{ m_String };
		std::size_t position{ 0 };

		while ((position = string.find(oldStr, position)) != std::string::npos)
		{
			string.replace(position, oldStr.size(), newStr);
			position += newStr.size();
		}

		return string;
	}

	Literal StringTrim::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::string string{ m_String };

		auto isWhiteSpace{
			[](unsigned char ch) { return std::isspace(ch); }
		};

		auto first{ std::find_if_not(string.begin(), string.end(), isWhiteSpace) };
		auto last{ std::find_if_not(string.rbegin(), string.rend(), isWhiteSpace).base() };

		if (first >= last)
			return std::string{};

		return std::string{ first, last };
	}

	Literal StringReverse::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::string string{ m_String };
		std::reverse(string.begin(), string.end());
		return string;
	}

	Literal StringSubstring::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& startIdxLiteral{ arguments[0] };
		const Literal& endIdxLiteral{ arguments[1] };

		std::size_t startIdx{ validateIndex(interpreter, startIdxLiteral) };
		std::size_t endIdx{ validateIndex(interpreter, endIdxLiteral) };

		if (startIdx > endIdx)
			throw interpreter.nativeError("start index cannot be greater than end index.");
		
		if (startIdx >= m_String.size())
			throw interpreter.nativeError("start index cannot be greater than the size of the string.");
		if (endIdx > m_String.size())
			throw interpreter.nativeError("end index cannot be greater than the size of the string.");

		if (startIdx == endIdx)
			return std::string{};

		return m_String.substr(startIdx, endIdx - startIdx);
	}

}