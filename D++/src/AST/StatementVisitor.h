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
	class BreakStmt;
	class ContinueStmt;
	class ReturnStmt;
	class FunctionStmt;
	class ClassStmt;

	//statement visitor interface
	class StmtVisitor
	{
	public:
		virtual ~StmtVisitor() = default;

		//visitors
		virtual void visitExpressionStmt(ExpressionStmt& expressionStmt) = 0;
		virtual void visitPrintStmt(PrintStmt& printStmt) = 0;
		virtual void visitVarStmt(VarStmt& varStmt) = 0;
		virtual void visitBlockStmt(BlockStmt& blockStmt) = 0;
		virtual void visitIfStmt(IfStmt& ifStmt) = 0;
		virtual void visitWhileStmt(WhileStmt& whileStmt) = 0;
		virtual void visitForStmt(ForStmt& forStmt) = 0;
		virtual void visitBreakStmt(BreakStmt& breakStmt) = 0;
		virtual void visitContinueStmt(ContinueStmt& continueStmt) = 0;
		virtual void visitReturnStmt(ReturnStmt& continueStmt) = 0;
		virtual void visitFunctionStmt(FunctionStmt& functionStmt) = 0;
		virtual void visitClassStmt(ClassStmt& classStmt) = 0;
	};

}