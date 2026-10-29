#pragma once

#include <string>

namespace DXX
{

	class LiteralExpr;
	class UnaryExpr;
	class BinaryExpr;
	class GroupingExpr;

	class ExprVisitor
	{
	public:
		virtual ~ExprVisitor() = default;

		virtual std::string visitLiteral (const LiteralExpr& literalExpr)  = 0;
		virtual std::string visitUnary   (const UnaryExpr& literalExpr)    = 0;
		virtual std::string visitBinary  (const BinaryExpr& literalExpr)   = 0;
		virtual std::string visitGrouping(const GroupingExpr& literalExpr) = 0;

	};

}