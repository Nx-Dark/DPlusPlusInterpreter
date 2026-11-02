#include "dpch.h"
#include "Statement.h"
#include "StatementVisitor.h"

namespace DXX
{

	void ExpressionStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitExpressionStmt(*this);
	}

	void PrintStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitPrintStmt(*this);
	}

	void VarStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitVarStmt(*this);
	}

	void BlockStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitBlockStmt(*this);
	}

	void IfStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitIfStmt(*this);
	}

	void WhileStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitWhileStmt(*this);
	}

	void ForStmt::Accept(StmtVisitor& visitor) const
	{
		visitor.visitForStmt(*this);
	}

}