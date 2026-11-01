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

}