#pragma once

namespace DXX
{

	class Stmt;
	class ExpressionStmt;
	class PrintStmt;
	class VarStmt;
	class BlockStmt;
	class IfStmt;
	class WhileStmt;
	class ForStmt;

	//statement visitor interface
	class StmtVisitor
	{
	public:
		virtual ~StmtVisitor() = default;

		//visitors
		virtual void visitExpressionStmt(const ExpressionStmt& expressionStmt) = 0;
		virtual void visitPrintStmt(const PrintStmt& printStmt) = 0;
		virtual void visitVarStmt(const VarStmt& varStmt) = 0;
		virtual void visitBlockStmt(const BlockStmt& blockStmt) = 0;
		virtual void visitIfStmt(const IfStmt& ifStmt) = 0;
		virtual void visitWhileStmt(const WhileStmt& whileStmt) = 0;
		virtual void visitForStmt(const ForStmt& forStmt) = 0;
	};

}