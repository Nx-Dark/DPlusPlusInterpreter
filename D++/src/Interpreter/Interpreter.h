#pragma once

#include "AST/EvaluatorVisitor.h"
#include "AST/StatementVisitor.h"

#include "Diagnostics/Diagnostic.h"
#include "Diagnostics/DiagnosticEngine.h"

namespace DXX
{

	struct Token;

	struct RuntimeError {};

	class Interpreter : public EvaluatorVisitor, public StmtVisitor
	{

	private:

		DiagnosticEngine& m_DiagnosticEngine;
		std::string m_SourceName{};

	public:
		Interpreter(DiagnosticEngine& engine, const std::string& sourceName);

		virtual ~Interpreter() = default;

		//entry point to the evalutaion
		void Interpret(const Expr& expr);
		void Interpret(const std::vector<Scope<Stmt>>& stmts);

		//expression visitor functions
		virtual Literal visitLiteral(const LiteralExpr& literalExpr) override;
		virtual Literal visitUnary(const UnaryExpr& unaryExpr) override;
		virtual Literal visitBinary(const BinaryExpr& binaryExpr) override;
		virtual Literal visitGrouping(const GroupingExpr& groupingExpr) override;

		//statement visitor functions;
		virtual void visitExpressionStmt(const ExpressionStmt& expressionStmt) override;
		virtual void visitPrintStmt(const PrintStmt& printStmt) override;

	private:
		RuntimeError error(const Token& tk, const std::string& msg);
	};

}