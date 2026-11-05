#pragma once

#include "AST/StatementResolverVisitor.h"
#include "AST/ExpressionResolverVisitor.h"

#include "Diagnostics/Diagnostic.h"
#include "Diagnostics/DiagnosticEngine.h"

namespace DXX
{

	class Interpreter;

	struct Token;

	struct ResolveError {};

	class Resolver : public ExprResolverVisitor, StmtResolverVisitor
	{
	private:

		DiagnosticEngine& m_DiagnosticEngine;
		Interpreter& m_Interpreter;
		std::string m_SourceName{};

		std::stack<std::unordered_map<std::string, bool>> m_Scopes{};

	public:

		Resolver(DiagnosticEngine& diagnosticEngine, Interpreter& interpreter);
		inline void SetSourceName(const std::string& sourceName) { m_SourceName = sourceName; }
		virtual ~Resolver();

		void Resolve(const std::vector<Ref<Stmt>>& statements);

		//visiting expressions
		virtual void visitLiteral(const LiteralExpr& literalExpr)  override {};
		virtual void visitUnary(const UnaryExpr& unaryExpr)  override;
		virtual void visitBinary(const BinaryExpr& binaryExpr)  override;
		virtual void visitGrouping(const GroupingExpr& groupingExpr)  override;
		virtual void visitVariable(const VariableExpr& variableExpr)  override;
		virtual void visitAssignment(const AssignExpr& assignExpr)  override;
		virtual void visitCall(const CallExpr& callExpr)  override;

		//visitng statements;
		virtual void visitExpressionStmt(ExpressionStmt& expressionStmt)  override;
		virtual void visitPrintStmt(PrintStmt& printStmt)  override;
		virtual void visitVarStmt(VarStmt& varStmt)  override;
		virtual void visitBlockStmt(BlockStmt& blockStmt)  override;
		virtual void visitIfStmt(IfStmt& ifStmt)  override;
		virtual void visitWhileStmt(WhileStmt& whileStmt)  override;
		virtual void visitForStmt(ForStmt& forStmt)  override;
		virtual void visitBreakStmt(BreakStmt& breakStmt)  override {};
		virtual void visitContinueStmt(ContinueStmt& continueStmt)  override {};
		virtual void visitReturnStmt(ReturnStmt& continueStmt)  override;
		virtual void visitFunctionStmt(FunctionStmt& functionStmt)  override;

	private:

		ResolveError error(Token token, const std::string& msg);

		void Resolve(const Expr& expr);
		void Resolve(const Ref<Stmt>& stmt);
		void ResolveLocal(const Expr& expr, Token name); //to resolve local variables inside a scope;
		void ResolveFunction(FunctionStmt& functionStmt);

		void BeginScope();
		void EndScope();

		void Declare(const Token& name);
		void Define(const Token& name);
	};

}