#pragma once

#if 1

#include "Expression.h"
#include "ExpressionVisitor.h"

namespace DXX
{

	class AstPrinter : public ExprVisitor
	{

	public:

		virtual std::string visitLiteral(const LiteralExpr& literalExpr) override;
		virtual std::string visitUnary(const UnaryExpr& unaryExpr) override;
		virtual std::string visitBinary(const BinaryExpr& binaryExpr) override;
		virtual std::string visitGrouping(const GroupingExpr& groupingExpr) override;

		std::string Print(const Expr& expr);

	};

}

#endif