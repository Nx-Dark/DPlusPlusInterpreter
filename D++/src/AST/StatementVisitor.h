#pragma once

namespace DXX
{

	class Stmt;
	class ExpressionStmt;
	class PrintStmt;
	class VarStmt;

	//statement visitor interface
	class StmtVisitor
	{
	public:
		virtual ~StmtVisitor() = default;

		//visitors
		virtual void visitExpressionStmt(const ExpressionStmt& expressionStmt) = 0;
		virtual void visitPrintStmt(const PrintStmt& printStmt) = 0;
		virtual void visitVarStmt(const VarStmt& varStmt) = 0;
	};

}