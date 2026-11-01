#include "dpch.h"
#include "Expression.h"
#include "ExpressionVisitor.h"
#include "EvaluatorVisitor.h"

namespace DXX
{

	//for AstPrinter, returns string;
	std::string LiteralExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitLiteral(*this);
	}

	std::string UnaryExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitUnary(*this);
	}

	std::string BinaryExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitBinary(*this);
	}

	std::string GroupingExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitGrouping(*this);
	}

	std::string VariableExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitVariable(*this);
	}

	std::string AssignExpr::Accept(ExprVisitor& visitor) const
	{
		return visitor.visitAssignment(*this);
	}

	//------------------------------------------------------------------------//

	//for Evaluator, returns Literal;
	Literal LiteralExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitLiteral(*this);
	}

	Literal UnaryExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitUnary(*this);
	}

	Literal BinaryExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitBinary(*this);
	}


	Literal GroupingExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitGrouping(*this);
	}

	Literal VariableExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitVariable(*this);
	}

	Literal AssignExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitAssignment(*this);
	}
}