#include "dpch.h"
#include "NativeFunctions.h"

#include "Interpreter/Interpreter.h"

namespace DXX
{
	//native clock function;
	Literal ClockFunction::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		auto timeNow{ std::chrono::system_clock::now() };
		double seconds{ (double)std::chrono::duration_cast<std::chrono::seconds>(timeNow.time_since_epoch()).count() };

		return seconds;
	}

	//native print function;

	Literal PrintFunction::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::string printString{};

		for (const auto& arg : arguments)
		{
			printString += LiteralToString(arg) + " ";
		}

		std::println("{0}", printString);

		return Nil{};
	}

	//native sqrt function

	Literal SqrtFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{

		const Literal arg1{ arguments[0] };

		if (std::holds_alternative<float>(arg1))
			return std::sqrtf(std::get<float>(arg1));
		else if (std::holds_alternative<double>(arg1))
			return std::sqrt(std::get<double>(arg1));
		else throw interpreter.nativeError("Unsupported Type as argument");

		return Nil{};
	}

	//native pow func

	Literal PowFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal arg1{ arguments[0] };
		const Literal arg2{ arguments[1] };

		if (std::holds_alternative<float>(arg1) && std::holds_alternative<float>(arg2))
			return std::powf(std::get<float>(arg1), std::get<float>(arg2));
		else if (std::holds_alternative<double>(arg1) && std::holds_alternative<double>(arg2))
			return std::powf(std::get<double>(arg1), std::get<double>(arg2));
		else throw interpreter.nativeError("Unsupported Type as argument");

		return Nil{};
	}

	//native max func
	Literal MaxFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal arg1{ arguments[0] };
		const Literal arg2{ arguments[1] };

		if (std::holds_alternative<float>(arg1) && std::holds_alternative<float>(arg2))
			return std::max(std::get<float>(arg1), std::get<float>(arg2));
		else if (std::holds_alternative<std::string>(arg1) && std::holds_alternative<std::string>(arg2))
			return std::max(std::get<std::string>(arg1), std::get<std::string>(arg2));
		else if (std::holds_alternative<double>(arg1) && std::holds_alternative<double>(arg2))
			return std::max(std::get<double>(arg1), std::get<double>(arg2));
		else throw interpreter.nativeError("Unsupported Type as argument");

		return Nil{};
	}
	//nativ min func
	Literal MinFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal arg1{ arguments[0] };
		const Literal arg2{ arguments[1] };

		if (std::holds_alternative<float>(arg1) && std::holds_alternative<float>(arg2))
			return std::min(std::get<float>(arg1), std::get<float>(arg2));
		else if (std::holds_alternative<std::string>(arg1) && std::holds_alternative<std::string>(arg2))
			return std::min(std::get<std::string>(arg1), std::get<std::string>(arg2));
		else if (std::holds_alternative<double>(arg1) && std::holds_alternative<double>(arg2))
			return std::min(std::get<double>(arg1), std::get<double>(arg2));
		else throw interpreter.nativeError("Unsupported Type as argument");

		return Nil{};
	}
}