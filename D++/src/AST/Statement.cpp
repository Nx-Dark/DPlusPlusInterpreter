#include "dpch.h"
#include "Statement.h"
#include "StatementVisitor.h"

namespace DXX
{

	void ExpressionStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitExpressionStmt(*this);
	}

	void PrintStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitPrintStmt(*this);
	}

	void VarStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitVarStmt(*this);
	}

	void BlockStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitBlockStmt(*this);
	}

	void IfStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitIfStmt(*this );
	}

	void WhileStmt::Accept(StmtVisitor& visitor )
	{
		visitor.visitWhileStmt(*this);
	}

	void ForStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitForStmt(*this);
	}

	void BreakStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitBreakStmt(*this);
	}

	void ContinueStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitContinueStmt(*this);
	}
	void ReturnStmt::Accept(StmtVisitor& visitor )
	{
		visitor.visitReturnStmt(*this);
	}

	void FunctionStmt::Accept(StmtVisitor& visitor)
	{
		visitor.visitFunctionStmt(*this);
	}

}