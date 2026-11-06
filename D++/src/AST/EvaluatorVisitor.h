#pragma once

#include "Types/Literal.h"

namespace DXX
{

	class Expr;
	class LiteralExpr;
	class UnaryExpr;
	class BinaryExpr;
	class GroupingExpr;
	class VariableExpr;
	class AssignExpr;
	class CallExpr;
	class GetExpr;
	class SetExpr;
	class ThisExpr;

	//visitor interface for evaluator visitors;
	class EvaluatorVisitor
	{
	public:
		virtual ~EvaluatorVisitor() = default;

		virtual Literal visitLiteral    (const LiteralExpr& literalExpr)   = 0;
		virtual Literal visitUnary      (const UnaryExpr& unaryExpr)       = 0;
		virtual Literal visitBinary     (const BinaryExpr& binaryExpr)     = 0;
		virtual Literal visitGrouping   (const GroupingExpr& groupingExpr) = 0;
		virtual Literal visitVariable   (const VariableExpr& variableExpr) = 0;
		virtual Literal visitAssignment (const AssignExpr& assignExpr)     = 0;
		virtual Literal visitCall       (const CallExpr& callExpr)	       = 0;
		virtual Literal visitGet		(const GetExpr& getExpr)		   = 0;
		virtual Literal visitSet		(const SetExpr& getExpr) = 0;
		virtual Literal visitThis		(const ThisExpr& thisExpr) = 0;
	};

}