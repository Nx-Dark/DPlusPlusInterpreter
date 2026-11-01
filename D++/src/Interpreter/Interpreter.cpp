#include "dpch.h"
#include "Interpreter.h"

#include "AST/Expression.h"
#include "AST/Statement.h"

namespace DXX
{

	static bool isTruthy(const Literal& literal)
	{
		if (std::holds_alternative<Nil>(literal)) return false;
		else if (std::holds_alternative<bool>(literal)) return std::get<bool>(literal);
		else return true;
	}

	Interpreter::Interpreter(DiagnosticEngine& engine, const std::string& sourceName)
		: m_DiagnosticEngine{ engine }, m_SourceName{ sourceName }
	{

	}

	void Interpreter::Interpret(const Expr& expr)
	{
		try {
			Literal literal{ expr.Accept(*this) };
			std::println("{0}", LiteralToString(literal));
		}
		catch (const RuntimeError&)
		{
			return;
		}
	}

	void Interpreter::Interpret(const std::vector<Scope<Stmt>>& stmts)
	{
		try {
			for (const auto& stmt : stmts)
			{
				stmt->Accept(*this);
			}
		}
		catch (const RuntimeError&)
		{
			return;
		}
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
				throw error(op, "Operation must done on a number.");
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
				throw error(op, "Unexpected Error Occured.");
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
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::MINUS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) - std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}
			case TokenType::STAR:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) * std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}
			case TokenType::SLASH:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) / std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}

			//logical and comparison operations;
			case TokenType::GREATER:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) > std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) > std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) < std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) < std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::GREATER_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) >= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) >= std::get<std::string>(rightOperand);
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) <= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) <= std::get<std::string>(rightOperand);
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::EQUAL_EQUAL: return leftOperand == rightOperand;
			case TokenType::BANG_EQUAL : return leftOperand != rightOperand;
			
			//logical operation
			case TokenType::AND: return rightOperand;
			case TokenType::OR : return rightOperand;

			default:
				throw error(op, "Unexpected Error Occured.");
		}

		return Nil{};
	}

	Literal Interpreter::visitGrouping(const GroupingExpr& groupingExpr)
	{
		return groupingExpr.m_Expr->Accept(*this);
	}

	//------------------------------------------------------------------

	void Interpreter::visitExpressionStmt(const ExpressionStmt& expressionStmt)
	{
		expressionStmt.m_Expr->Accept(*this);
	}

	void Interpreter::visitPrintStmt(const PrintStmt& printStmt)
	{
		Literal literal{ printStmt.m_Expr->Accept(*this) };

		std::println("{0}", LiteralToString(literal));
	}

	//runtime error handling
	RuntimeError Interpreter::error(const Token& tk, const std::string& msg)
	{
		Diagnostic diag{
			DiagnosticSeverity::ERROR,
			DiagnosticPhase::RUNTIME,
			msg,
			{ m_SourceName, tk.m_Line, tk.m_Column, 0 }
		};

		m_DiagnosticEngine.Report(std::move(diag));

		return RuntimeError{};
	}
}