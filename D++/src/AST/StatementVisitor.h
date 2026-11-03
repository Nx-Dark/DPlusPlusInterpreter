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

	//statement visitor interface
	class StmtVisitor
	{
	public:
		virtual ~StmtVisitor() = default;

		//visitors
		virtual void visitExpressionStmt(ExpressionStmt& expressionStmt, Scope<Stmt>& statement)  = 0;
		virtual void visitPrintStmt		(PrintStmt& printStmt, Scope<Stmt>&           statement)  = 0;
		virtual void visitVarStmt		(VarStmt& varStmt, Scope<Stmt>&				stateement)	= 0;
		virtual void visitBlockStmt		(BlockStmt& blockStmt, Scope<Stmt>&           statement)	= 0;
		virtual void visitIfStmt		(IfStmt& ifStmt, Scope<Stmt>&                 statement)	= 0;
		virtual void visitWhileStmt		(WhileStmt& whileStmt, Scope<Stmt>&			statement)	= 0;
		virtual void visitForStmt		(ForStmt& forStmt, Scope<Stmt>&				statement)	= 0;
		virtual void visitBreakStmt		(BreakStmt& breakStmt, Scope<Stmt>&			statement)	= 0;
		virtual void visitContinueStmt	(ContinueStmt& continueStmt, Scope<Stmt>&		statement)	= 0;
		virtual void visitReturnStmt    (ReturnStmt& continueStmt, Scope<Stmt>& statement) = 0;
		virtual void visitFunctionStmt	(FunctionStmt& functionStmt, Scope<Stmt>&		statement)	= 0;
	};

}