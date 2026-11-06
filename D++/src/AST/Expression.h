#pragma once

#include "Tokenizer/Token.h"

#include "Core/Core.h"

#include "IdType.h"

namespace DXX

{

	class ExprVisitor;
	class EvaluatorVisitor;
	class ExprResolverVisitor;

	//base expression interface
	class Expr
	{
		ExprID m_ID{};

		inline static ExprID s_NextID{ 0 };

	public:
		Expr();
		virtual ~Expr() = default;

		virtual Literal Accept(EvaluatorVisitor& visitor) const = 0;
		virtual void Accept(ExprResolverVisitor& visitor) const = 0;

		inline ExprID GetID() const { return m_ID; }
	};

	//literal expression
	class LiteralExpr : public Expr
	{
	public:
		Literal m_Literal{};

	public:
		LiteralExpr(Literal literal)
			: m_Literal{ std::move(literal) } {}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};
	
	//unary expression
	class UnaryExpr : public Expr
	{
	public:
		Token m_Operator;
		Ref<Expr> m_Expr{};

	public:
		UnaryExpr(Token op, Ref<Expr> expr)
			: m_Expr{ expr }, m_Operator{ std::move(op) } { }

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	//binary expression
	class BinaryExpr : public Expr
	{
	public:
		Ref<Expr> m_LeftExpr{};
		Token m_Operator;
		Ref<Expr> m_RightExpr{};

	public:
		BinaryExpr(Ref<Expr> leftExpr, Token op, Ref<Expr> rightExpr)
			: m_LeftExpr{ leftExpr }, m_Operator{ std::move(op) }, m_RightExpr{ rightExpr } { }

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;

	};

	//grouped or grouping expressions
	class GroupingExpr : public Expr
	{
	public:
		Ref<Expr> m_Expr{};

	public:
		GroupingExpr(Ref<Expr> expr)
			: m_Expr{ expr } { }

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	//more stuff
	class VariableExpr : public Expr
	{
	public:
		Token m_Token;

	public:
		VariableExpr(Token token)
			: m_Token{ std::move(token) } { }

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class AssignExpr : public Expr
	{
	public:
		Token m_Identifier;
		Ref<Expr> m_Expr{};

	public:
		AssignExpr(Token identifier, Ref<Expr> expr)
			: m_Identifier{ std::move(identifier) }, m_Expr{ expr } 
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class CallExpr : public Expr
	{
	public:
		Ref<Expr> m_Callee{};
		std::vector<Ref<Expr>> m_Arguments{};
		Token m_ClosingParen{};

	public:
		CallExpr(Ref<Expr> callee, std::vector<Ref<Expr>> args, Token closingParen)
			: m_Callee{ callee }, m_Arguments{ std::move(args) }, m_ClosingParen{ std::move(closingParen) }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class GetExpr : public Expr
	{
	public:
		Ref<Expr> m_Object{};
		Token m_Identifier{};

	public:
		GetExpr(Ref<Expr> obj, Token identifier)
			: m_Object{ obj }, m_Identifier{ std::move(identifier) }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class SetExpr : public Expr
	{
	public:
		Ref<Expr> m_Object{};
		Token m_Identifier{};
		Ref<Expr> m_Value{};

	public:
		SetExpr(Ref<Expr> obj, Token identifier, Ref<Expr> value)
			: m_Object{ obj }, m_Identifier{ std::move(identifier) }, m_Value{ value }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class ThisExpr : public Expr
	{
	public:
		Token m_Keyword{};

	public:
		ThisExpr(Token keyword)
			: m_Keyword{std::move(keyword)}
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

}