#include "dpch.h"
#include "NativeFunctions.h"

#include "Interpreter/Interpreter.h"
#include "Callable/Classes/ClassInstance.h"

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
		else throw interpreter.nativeError("Unsupported Type as argument.");

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
		else throw interpreter.nativeError("Unsupported Type as argument.");

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
		else throw interpreter.nativeError("Unsupported Type as argument.");

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
		else throw interpreter.nativeError("Unsupported Type as argument.");

		return Nil{};
	}

	//native input func
	Literal InputFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal prompt{ arguments[0] };
		std::string line{};

		if (std::holds_alternative<std::string>(prompt))
		{
			std::print("{0}", std::get<std::string>(prompt));
			if (std::getline(std::cin, line))
				return line;
		}

		else throw interpreter.nativeError("Unsupported Type as argument.");
	}

	//casting functions;
	//string cast;
	Literal StrCastFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal toCastValue{ arguments[0] };

		if (std::holds_alternative<std::string>(toCastValue))
			return std::get<std::string>(toCastValue);
		else if (std::holds_alternative<float>(toCastValue))
			return std::to_string(std::get<float>(toCastValue));
		else if (std::holds_alternative<double>(toCastValue))
			return std::to_string(std::get<double>(toCastValue));
		else if (std::holds_alternative<bool>(toCastValue))
		{
			if (std::get<bool>(toCastValue) == true) return "True";
			else return "False";
		}
		else if (std::holds_alternative<Nil>(toCastValue))
			return "NIL";
		else
			throw interpreter.nativeError("Unsupported Type to Cast.");
	}

	//float cast
	Literal FloatCastFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal toCastValue{ arguments[0] };

		if (std::holds_alternative<std::string>(toCastValue))
		{
			float val{};
			try {
				val = std::stof(std::get<std::string>(toCastValue));
				return val;
			}
			catch (const std::invalid_argument& e)
			{
				throw interpreter.nativeError("Invalid cast: Cannot cast string to float.");
			}
			catch (const std::out_of_range& e)
			{
				throw interpreter.nativeError("String is too big to cast to float.");
			}
		}
		else if (std::holds_alternative<float>(toCastValue))
			return std::get<float>(toCastValue);
		else if (std::holds_alternative<double>(toCastValue))
			return static_cast<float>(std::get<double>(toCastValue));
		else if (std::holds_alternative<bool>(toCastValue))
		{
			if (std::get<bool>(toCastValue) == true) return 1.0f;
			else return 0.0f;
		}
		else if (std::holds_alternative<Nil>(toCastValue))
			throw interpreter.nativeError("Invalid cast: cannot cast NIL to float.");
		else
			throw interpreter.nativeError("Unsupported Type to Cast.");
	}

	//float cast
	Literal DoubleCastFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal toCastValue{ arguments[0] };

		if (std::holds_alternative<std::string>(toCastValue))
		{
			double val{};
			try {
				val = std::stod(std::get<std::string>(toCastValue));
				return val;
			}
			catch (const std::invalid_argument&)
			{
				throw interpreter.nativeError("Invalid cast: Cannot cast string to double.");
			}
			catch (const std::out_of_range&)
			{
				throw interpreter.nativeError("String is too big to cast to double.");
			}
		}
		else if (std::holds_alternative<float>(toCastValue))
			return static_cast<double>(std::get<float>(toCastValue));
		else if (std::holds_alternative<double>(toCastValue))
			return std::get<double>(toCastValue);
		else if (std::holds_alternative<bool>(toCastValue))
		{
			if (std::get<bool>(toCastValue) == true) return 1.0;
			else return 0.0;
		}
		else if (std::holds_alternative<Nil>(toCastValue))
			throw interpreter.nativeError("Invalid cast: cannot cast NIL to double.");
		else
			throw interpreter.nativeError("Unsupported Type to Cast.");
	}

	//type check
	Literal TypeCheckFunc::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal value{ arguments[0] };

		if (std::holds_alternative<float>(value))
			std::println("<float>");
		else if (std::holds_alternative<double>(value))
			std::println("<double>");
		else if (std::holds_alternative<std::string>(value))
			std::println("<str>");
		else if (std::holds_alternative<bool>(value))
			std::println("<bool>");
		else if (std::holds_alternative<Nil>(value))
			std::println("<NIL>");
		else if (std::holds_alternative<Ref<Callable>>(value))
			std::println("{0}", std::get<Ref<Callable>>(value)->ToString());
		else if (std::holds_alternative<Ref<ClassInstance>>(value))
			std::println("{0}", std::get<Ref<ClassInstance>>(value)->ToString());
		else throw interpreter.nativeError("Unknown type.");

		return Nil{};
	}
}