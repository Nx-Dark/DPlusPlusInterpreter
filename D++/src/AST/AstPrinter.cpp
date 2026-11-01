#include "dpch.h"

#if 1

#include "AstPrinter.h"

namespace DXX
{

	std::string AstPrinter::visitLiteral(const LiteralExpr& literalExpr)
	{
		return LiteralToString(literalExpr.m_Literal);
	}

	std::string AstPrinter::visitUnary(const UnaryExpr& unaryExpr)
	{
		return "(" + unaryExpr.m_Operator.m_Lexeme + " " + unaryExpr.m_Expr->Accept(*this) + ")";
	}

	std::string AstPrinter::visitBinary(const BinaryExpr& binaryExpr)
	{
		return "(" + binaryExpr.m_Operator.m_Lexeme + " " 
			+ binaryExpr.m_LeftExpr->Accept(*this) + " " + binaryExpr.m_RightExpr->Accept(*this) + ")";
	}

	std::string AstPrinter::visitGrouping(const GroupingExpr& groupingExpr)
	{
		return "(group " + groupingExpr.m_Expr->Accept(*this) + ")";
	}

	std::string AstPrinter::visitVariable(const VariableExpr& variableExpr)
	{
		return variableExpr.m_Token.m_Lexeme;
	}

	std::string AstPrinter::visitAssignment(const AssignExpr& assignExpr)
	{
		return assignExpr.m_Identifier.m_Lexeme + " = " + assignExpr.Accept(*this);
	}

	std::string AstPrinter::Print(const Expr& expr)
	{
		return expr.Accept(*this);
	}

}

#endif