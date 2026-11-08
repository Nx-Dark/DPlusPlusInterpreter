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
		UnaryExpr(const Token& op, Ref<Expr> expr)
			: m_Expr{ expr }, m_Operator{ op } { }

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
		BinaryExpr(Ref<Expr> leftExpr, const Token& op, Ref<Expr> rightExpr)
			: m_LeftExpr{ leftExpr }, m_Operator{ op }, m_RightExpr{ rightExpr } { }

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
		VariableExpr(const Token& token)
			: m_Token{ token } { }

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class AssignExpr : public Expr
	{
	public:
		Token m_Identifier;
		Ref<Expr> m_Expr{};

	public:
		AssignExpr(const Token& identifier, Ref<Expr> expr)
			: m_Identifier{ identifier }, m_Expr{ expr } 
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
		CallExpr(Ref<Expr> callee, std::vector<Ref<Expr>> args, const Token& closingParen)
			: m_Callee{ callee }, m_Arguments{ std::move(args) }, m_ClosingParen{ closingParen }
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
		GetExpr(Ref<Expr> obj, const Token& identifier)
			: m_Object{ obj }, m_Identifier{ identifier }
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
		Token m_Operator{};

	public:
		SetExpr(Ref<Expr> obj, const Token& identifier, Ref<Expr> value, const Token& op)
			: m_Object{ obj }, m_Identifier{ identifier }, m_Value{ value }, m_Operator{ op }
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
		ThisExpr(const Token& keyword)
			: m_Keyword{ keyword }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class SuperExpr : public Expr
	{
	public:
		Token m_Keyword{};
		Token m_Method{};

	public:
		SuperExpr(const Token& keyword, const Token& method)
			: m_Keyword{ keyword }, m_Method{ method }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class ListExpr : public Expr
	{
	public:
		std::vector<Ref<Expr>> m_Elements{};

	public:
		ListExpr(std::vector<Ref<Expr>> elements)
			: m_Elements{ std::move(elements) }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class IndexExpr : public Expr
	{
	public:
		Ref<Expr> m_Object{};
		Ref<Expr> m_Index{};
		Token m_keyword{}; //for error and stuff handling

	public:
		IndexExpr(Ref<Expr> object, Ref<Expr> index, const Token& keyword)
			: m_Object{ object }, m_Index{ index }, m_keyword{ keyword }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

	class IndexSetExpr : public Expr
	{
	public:
		Ref<Expr> m_Object{};
		Ref<Expr> m_Index{};
		Ref<Expr> m_Value{};
		Token m_Operator;

	public:
		IndexSetExpr(Ref<Expr> object, Ref<Expr> index, Ref<Expr> value, const Token& op)
			: m_Object{ object }, m_Index{ index }, m_Value{ value }, m_Operator{ op }
		{
		}

		virtual Literal Accept(EvaluatorVisitor& visitor) const override;
		virtual void Accept(ExprResolverVisitor& visitor) const override;
	};

}