#include "dpch.h"
#include "Expression.h"
#include "EvaluatorVisitor.h"
#include "ExpressionResolverVisitor.h"

namespace DXX
{
	//unique id for every expression
	Expr::Expr()
		: m_ID{ s_NextID++ }
	{
	}

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

	Literal CallExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitCall(*this);
	}

	Literal GetExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitGet(*this);
	}

	Literal SetExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitSet(*this);
	}

	Literal ThisExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitThis(*this);
	}

	Literal SuperExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitSuper(*this);
	}

	Literal ListExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitList(*this);
	}

	Literal IndexExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitIndex(*this);
	}

	Literal IndexSetExpr::Accept(EvaluatorVisitor& visitor) const
	{
		return visitor.visitIndexSet(*this);
	}

	//for resolver, returns nothing(void);
	void LiteralExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitLiteral(*this);
	}

	void UnaryExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitUnary(*this);
	}

	void BinaryExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitBinary(*this);
	}

	void GroupingExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitGrouping(*this);
	}

	void VariableExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitVariable(*this);
	}

	void AssignExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitAssignment(*this);
	}

	void CallExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitCall(*this);
	}

	void GetExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitGet(*this);
	}

	void SetExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitSet(*this);
	}

	void ThisExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitThis(*this);
	}

	void SuperExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitSuper(*this);
	}

	void ListExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitList(*this);
	}

	void IndexExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitIndex(*this);
	}

	void IndexSetExpr::Accept(ExprResolverVisitor& visitor) const
	{
		visitor.visitIndexSet(*this);
	}
}