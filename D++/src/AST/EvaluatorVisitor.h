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
	class SuperExpr;
	class ListExpr;
	class IndexExpr;
	class IndexSetExpr;

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
		virtual Literal visitSet		(const SetExpr& getExpr)		   = 0;
		virtual Literal visitThis		(const ThisExpr& thisExpr)		   = 0;
		virtual Literal visitSuper		(const SuperExpr& superExpr)	   = 0;
		virtual Literal visitList		(const ListExpr& listExpr)		   = 0;
		virtual Literal visitIndex		(const IndexExpr& indexExpr)	   = 0;
		virtual Literal visitIndexSet	(const IndexSetExpr& indexSetExpr) = 0;
	};

}