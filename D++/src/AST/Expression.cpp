#include "dpch.h"
#include "Expression.h"
#include "ExpressionVisitor.h"

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

}