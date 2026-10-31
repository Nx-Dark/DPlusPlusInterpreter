#pragma once

#include "Tokenizer/Token.h"

#include "Core/Core.h"

namespace DXX
{
	class ExprVisitor;
	class EvaluatorVisitor;

	//base expression interface
	class Expr
	{
	public:
		virtual ~Expr() = default;

		virtual std::string Accept(ExprVisitor& visitor) const = 0;
		virtual Literal Accept(EvaluatorVisitor& visitor) const = 0;
	};

	//literal expression
	class LiteralExpr : public Expr
	{
	public:
		Literal m_Literal{};

	public:
		LiteralExpr(Literal literal)
			: m_Literal{ std::move(literal) } {}

		virtual std::string Accept(ExprVisitor& visitor) const override;
		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
	};
	
	//unary expression
	class UnaryExpr : public Expr
	{
	public:
		Token m_Operator;
		Scope<Expr> m_Expr{};

	public:
		UnaryExpr(Token op, Scope<Expr> expr)
			: m_Expr{ std::move(expr) }, m_Operator{ std::move(op) } { }

		virtual std::string Accept(ExprVisitor& visitor) const override;
		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
	};

	//binary expression
	class BinaryExpr : public Expr
	{
	public:
		Scope<Expr> m_LeftExpr{};
		Token m_Operator;
		Scope<Expr> m_RightExpr{};

	public:
		BinaryExpr(Scope<Expr> leftExpr, Token op, Scope<Expr> rightExpr)
			: m_LeftExpr{ std::move(leftExpr) }, m_Operator{ std::move(op) }, m_RightExpr{ std::move(rightExpr) } { }

		virtual std::string Accept(ExprVisitor& visitor) const override;
		virtual Literal Accept(EvaluatorVisitor& visitor) const override;

	};

	//grouped or grouping expressions
	class GroupingExpr : public Expr
	{
	public:
		Scope<Expr> m_Expr{};

	public:
		GroupingExpr(Scope<Expr> expr)
			: m_Expr{ std::move(expr) } { }

		virtual std::string Accept(ExprVisitor& visitor) const override;
		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
	};

}