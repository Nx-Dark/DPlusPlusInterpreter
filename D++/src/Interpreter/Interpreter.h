#pragma once

#include "AST/EvaluatorVisitor.h"
#include "AST/StatementVisitor.h"

#include "Diagnostics/Diagnostic.h"
#include "Diagnostics/DiagnosticEngine.h"

#include "Environment.h"

namespace DXX
{

	struct Token;

	struct RuntimeError {};

	class Interpreter : public EvaluatorVisitor, public StmtVisitor
	{

	private:

		DiagnosticEngine& m_DiagnosticEngine;
		std::string m_SourceName{};

		//environment;
		Environment m_Environment{};

	public:
		Interpreter(DiagnosticEngine& engine);
		inline void SetSourceName(const std::string& sourceName) { m_SourceName = sourceName; }

		virtual ~Interpreter() = default;

		//entry point to the evalutaion
		void Interpret(const Expr& expr);
		void Interpret(const std::vector<Scope<Stmt>>& stmts);

		//expression visitor functions
		virtual Literal visitLiteral(const LiteralExpr& literalExpr) override;
		virtual Literal visitUnary(const UnaryExpr& unaryExpr) override;
		virtual Literal visitBinary(const BinaryExpr& binaryExpr) override;
		virtual Literal visitGrouping(const GroupingExpr& groupingExpr) override;
		virtual Literal visitVariable(const VariableExpr& variableExpr) override;
		virtual Literal visitAssignment(const AssignExpr& assignExpr) override;

		//statement visitor functions;
		virtual void visitExpressionStmt(const ExpressionStmt& expressionStmt) override;
		virtual void visitPrintStmt(const PrintStmt& printStmt) override;
		virtual void visitVarStmt(const VarStmt& varStmt) override;

	private:
		RuntimeError error(const Token& tk, const std::string& msg);
	};

}