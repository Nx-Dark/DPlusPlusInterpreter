#pragma once

#include "Tokenizer/Literal.h"

namespace DXX
{

	class LiteralExpr;
	class UnaryExpr;
	class BinaryExpr;
	class GroupingExpr;

	//visitor interface for evaluator visitors;
	class EvaluatorVisitor
	{
	public:
		virtual ~EvaluatorVisitor() = default;

		virtual Literal visitLiteral (const LiteralExpr& literalExpr) = 0;
		virtual Literal visitUnary   (const UnaryExpr& unaryExpr) = 0;
		virtual Literal visitBinary  (const BinaryExpr& binaryExpr) = 0;
		virtual Literal visitGrouping(const GroupingExpr& groupingExpr) = 0;
	};

}