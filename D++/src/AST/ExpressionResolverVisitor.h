#pragma once

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

	//visitor interface for evaluator visitors;
	class ExprResolverVisitor
	{
	public:
		virtual ~ExprResolverVisitor() = default;

		virtual void visitLiteral(const LiteralExpr& literalExpr) = 0;
		virtual void visitUnary(const UnaryExpr& unaryExpr) = 0;
		virtual void visitBinary(const BinaryExpr& binaryExpr) = 0;
		virtual void visitGrouping(const GroupingExpr& groupingExpr) = 0;
		virtual void visitVariable(const VariableExpr& variableExpr) = 0;
		virtual void visitAssignment(const AssignExpr& assignExpr) = 0;
		virtual void visitCall(const CallExpr& callExpr) = 0;
	};

}