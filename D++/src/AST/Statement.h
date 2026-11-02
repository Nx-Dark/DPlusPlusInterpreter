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

	//block stmt
	class BlockStmt : public Stmt
	{
	public:
		std::vector<Scope<Stmt>> m_Statements{};

	public:
		BlockStmt(std::vector<Scope<Stmt>> statements)
			: m_Statements{ std::move(statements) } 
		{
		}

		virtual void Accept(StmtVisitor& visitor) const override;
	};

	//conditional statements
	class IfStmt : public Stmt
	{
	public:
		Scope<Expr> m_Condition{};
		Scope<Stmt> m_ThenBranch{};
		Scope<Stmt> m_ElseBranch{};

	public:
		IfStmt(Scope<Expr> condition, Scope<Stmt> thenBranch, Scope<Stmt> elseBranch)
			: m_Condition{ std::move(condition) }, m_ThenBranch{ std::move(thenBranch) }, m_ElseBranch{ std::move(elseBranch) }
		{ }

		virtual void Accept(StmtVisitor& visitor) const override;
	};

	class WhileStmt : public Stmt
	{
	public:
		Scope<Expr> m_Condition{};
		Scope<Stmt> m_ThenBranch{};

	public:
		WhileStmt(Scope<Expr> condition, Scope<Stmt> thenBranch)
			: m_Condition{ std::move(condition) }, m_ThenBranch{ std::move(thenBranch) }
		{
		}

		virtual void Accept(StmtVisitor& visitor) const override;
	};

	class ForStmt : public Stmt
	{
	public:
		Scope<Stmt> m_Initializer{};
		Scope<Expr> m_Condition{};
		Scope<Expr> m_Increment{};
		Scope<Stmt> m_Body{};

	public:
		ForStmt(Scope<Stmt> initializer, Scope<Expr> condition, Scope<Expr> increment, Scope<Stmt> body)
			: m_Initializer{ std::move(initializer) }, m_Condition{ std::move(condition) }, m_Increment{ std::move(increment) }, m_Body{ std::move(body) }
		{ }

		virtual void Accept(StmtVisitor& visitor) const override;
	};

}