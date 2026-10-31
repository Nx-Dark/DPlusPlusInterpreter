#pragma once

#include "AST/EvaluatorVisitor.h"

namespace DXX
{

	class Interpreter : public EvaluatorVisitor
	{

	public:

		virtual ~Interpreter() = default;

		//expression evalutaion;
		virtual Literal visitLiteral(const LiteralExpr& literalExpr) override;
		virtual Literal visitUnary(const UnaryExpr& unaryExpr) override;
		virtual Literal visitBinary(const BinaryExpr& binaryExpr) override;
		virtual Literal visitGrouping(const GroupingExpr& groupingExpr) override;

	};

}