#include "dpch.h"
#include "Statement.h"
#include "StatementVisitor.h"

namespace DXX
{

	void ExpressionStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitExpressionStmt(*this, self);
	}

	void PrintStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitPrintStmt(*this, self);
	}

	void VarStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitVarStmt(*this, self);
	}

	void BlockStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitBlockStmt(*this, self);
	}

	void IfStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitIfStmt(*this, self);
	}

	void WhileStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitWhileStmt(*this, self);
	}

	void ForStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitForStmt(*this, self);
	}

	void BreakStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitBreakStmt(*this, self);
	}

	void ContinueStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitContinueStmt(*this, self);
	}

	void FunctionStmt::Accept(StmtVisitor& visitor, Scope<Stmt>& self)
	{
		visitor.visitFunctionStmt(*this, self);
	}

}