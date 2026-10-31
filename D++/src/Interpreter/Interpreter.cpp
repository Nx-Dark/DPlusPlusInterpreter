#include "dpch.h"
#include "Interpreter.h"

#include "AST/Expression.h"

namespace DXX
{

	static bool isTruthy(const Literal& literal)
	{
		if (std::holds_alternative<Nil>(literal)) return false;
		else if (std::holds_alternative<bool>(literal)) return std::get<bool>(literal);
		else return true;
	}

	Literal Interpreter::visitLiteral(const LiteralExpr& literalExpr)
	{
		return literalExpr.m_Literal;
	}

	Literal Interpreter::visitUnary(const UnaryExpr& unaryExpr)
	{
		Literal operand{ unaryExpr.m_Expr->Accept(*this) };
		const Token& op{ unaryExpr.m_Operator };

		switch (op.m_Type)
		{
			case TokenType::MINUS: {
				if (std::holds_alternative<float>(operand))
					return -(std::get<float>(operand));
				break;
			}
			case TokenType::BANG:
			{
				if (std::holds_alternative<Nil>(operand))
					return true;
				else if (std::holds_alternative<bool>(operand))
					return !(std::get<bool>(operand));
				else
					return false;
			}
			default:
				std::println("Error"); //TODO: report runtime error;
				break;
		}

		return Nil{};
	}

	Literal Interpreter::visitBinary(const BinaryExpr& binaryExpr)
	{
		const Token& op{ binaryExpr.m_Operator };
		Literal leftOperand{ binaryExpr.m_LeftExpr->Accept(*this) };

		//for evalutaing 'and' and 'or' logical operators;
		if (op.m_Type == TokenType::AND)
		{
			if (!isTruthy(leftOperand)) return leftOperand;
		}
		else if (op.m_Type == TokenType::OR)
		{
			if (isTruthy(leftOperand)) return leftOperand;
		}

		//evaluate rhs after
		Literal rightOperand{ binaryExpr.m_RightExpr->Accept(*this) };

		switch (op.m_Type)
		{
			//arithmetic operations;
			case TokenType::PLUS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) + std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) + std::get<std::string>(rightOperand);
				break;
			}
			case TokenType::MINUS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) - std::get<float>(rightOperand);
				break;
			}
			case TokenType::STAR:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) * std::get<float>(rightOperand);
				break;
			}
			case TokenType::SLASH:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) / std::get<float>(rightOperand);
				break;
			}

			//logical and comparison operations;
			case TokenType::GREATER:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) > std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) > std::get<std::string>(rightOperand);
				break;
			}
			case TokenType::LESS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) < std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) < std::get<std::string>(rightOperand);
				break;
			}
			case TokenType::GREATER_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) >= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) >= std::get<std::string>(rightOperand);
				break;
			}
			case TokenType::LESS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) <= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) <= std::get<std::string>(rightOperand);
				break;
			}
			case TokenType::EQUAL_EQUAL: return leftOperand == rightOperand;
			case TokenType::BANG_EQUAL : return leftOperand != rightOperand;
			
			//logical operation
			case TokenType::AND: return rightOperand;
			case TokenType::OR : return rightOperand;

			default:
				std::println("Error"); //TODO: report runtime error;
				break;
		}

		return Nil{};
	}

	Literal Interpreter::visitGrouping(const GroupingExpr& groupingExpr)
	{
		return groupingExpr.m_Expr->Accept(*this);
	}
}