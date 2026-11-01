#pragma once

#include "Expression.h"

namespace DXX
{
	//statement class containing all types of statements the lang supports;

	class StmtVisitor;

	class Stmt
	{
	public:
		virtual ~Stmt() = default;

		virtual void Accept(StmtVisitor& visitor) const = 0;
	};

	//expressionstmt
	class ExpressionStmt : public Stmt
	{
	public:
		Scope<Expr> m_Expr{};

	public:
		ExpressionStmt(Scope<Expr> expr)
			: m_Expr{ std::move(expr) } 
		{
		}

		virtual void Accept(StmtVisitor& visitor) const override;

	};

	//print stmt
	class PrintStmt : public Stmt
	{
	public:
		Scope<Expr> m_Expr{};

	public:
		PrintStmt(Scope<Expr> expr)
			: m_Expr{ std::move(expr) }
		{
		}

		virtual void Accept(StmtVisitor& visitor) const override;
	};

	//var stmt
	class VarStmt : public Stmt
	{
	public:
		Token m_Token;
		Scope<Expr> m_InitializerExpr{};

	public:
		VarStmt(Token token, Scope<Expr> initializerExpr = nullptr)
			: m_Token{ std::move(token) }, m_InitializerExpr{ std::move(initializerExpr) }
		{
		}

		virtual void Accept(StmtVisitor& visitor) const override;

	};

}