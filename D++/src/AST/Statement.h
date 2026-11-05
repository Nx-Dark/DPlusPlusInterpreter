#pragma once

#include "IdType.h"
#include "Expression.h"

namespace DXX
{
	//statement class containing all types of statements the lang supports;

	class StmtVisitor;
	class StmtResolverVisitor;

	class Stmt
	{
		StmtID m_ID{};

		inline static StmtID s_NextID{ 0 };

	public:
		Stmt();
		virtual ~Stmt() = default;

		virtual void Accept(StmtVisitor& visitor) = 0;
		virtual void Accept(StmtResolverVisitor& visitor) = 0;

		inline StmtID GetID() const { return m_ID; }
	};

	//expressionstmt
	class ExpressionStmt : public Stmt
	{
	public:
		Ref<Expr> m_Expr{};

	public:
		ExpressionStmt(Ref<Expr> expr)
			: m_Expr{ expr } 
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;

	};

	//print stmt
	class PrintStmt : public Stmt
	{
	public:
		Ref<Expr> m_Expr{};

	public:
		PrintStmt(Ref<Expr> expr)
			: m_Expr{ expr }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	//var stmt
	class VarStmt : public Stmt
	{
	public:
		Token m_Token{};
		Ref<Expr> m_InitializerExpr{};

	public:
		VarStmt(Token token, Ref<Expr> initializerExpr = nullptr)
			: m_Token{ std::move(token) }, m_InitializerExpr{ initializerExpr }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;

	};

	//block stmt
	class BlockStmt : public Stmt
	{
	public:
		std::vector<Ref<Stmt>> m_Statements{};

	public:
		BlockStmt(std::vector<Ref<Stmt>> statements)
			: m_Statements{ std::move(statements) } 
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	//conditional statements
	class IfStmt : public Stmt
	{
	public:
		Ref<Expr> m_Condition{};
		Ref<Stmt> m_ThenBranch{};
		Ref<Stmt> m_ElseBranch{};

	public:
		IfStmt(Ref<Expr> condition, Ref<Stmt> thenBranch, Ref<Stmt> elseBranch)
			: m_Condition{ condition }, m_ThenBranch{ thenBranch }, m_ElseBranch{ elseBranch }
		{ }

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	class WhileStmt : public Stmt
	{
	public:
		Ref<Expr> m_Condition{};
		Ref<Stmt> m_ThenBranch{};

	public:
		WhileStmt(Ref<Expr> condition, Ref<Stmt> thenBranch)
			: m_Condition{ condition }, m_ThenBranch{ thenBranch }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	class ForStmt : public Stmt
	{
	public:
		Ref<Stmt> m_Initializer{};
		Ref<Expr> m_Condition{};
		Ref<Expr> m_Increment{};
		Ref<Stmt> m_Body{};

	public:
		ForStmt(Ref<Stmt> initializer, Ref<Expr> condition, Ref<Expr> increment, Ref<Stmt> body)
			: m_Initializer{ initializer }, m_Condition{ condition }, m_Increment{ increment }, m_Body{ body }
		{ }

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	//control statements;
	class BreakStmt : public Stmt
	{
	public:

	public:

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	class ContinueStmt : public Stmt
	{
	public:

	public:

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	class ReturnStmt : public Stmt
	{
	public:
		Token m_Keyword{};
		Ref<Expr> m_Expr{};

	public:
		ReturnStmt(Token keyword, Ref<Expr> expr)
			: m_Keyword{ std::move(keyword) }, m_Expr{ expr }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	//functions
	class FunctionStmt : public Stmt
	{
	public:
		Token m_Name{};
		Ref<Stmt> m_Body{};
		std::vector<Token> m_Parameters{};

	public:
		FunctionStmt(Token name, Ref<Stmt> body, std::vector<Token> parameters = {})
			: m_Name{ std::move(name) }, m_Body{ body }, m_Parameters{ std::move(parameters) }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

	class ClassStmt : public Stmt
	{
	public:
		Token m_Name{};
		std::vector<Ref<Stmt>> m_Methods{};

	public:
		ClassStmt(Token name, std::vector<Ref<Stmt>> methods)
			: m_Name{ std::move(name) }, m_Methods{ std::move(methods) }
		{
		}

		virtual void Accept(StmtVisitor& visitor) override;
		virtual void Accept(StmtResolverVisitor& visitor) override;
	};

}