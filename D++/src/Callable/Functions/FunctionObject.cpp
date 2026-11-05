#include "dpch.h"
#include "FunctionObject.h"

#include "AST/Statement.h"
#include "Interpreter/Environment.h"
#include "Interpreter/Interpreter.h"

namespace DXX
{

	FunctionObject::FunctionObject(Ref<FunctionStmt> decl, const Ref<Environment>& closure)
		: m_Declaration{ decl }, m_Closure{ closure } 
	{
	}

	Literal FunctionObject::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		EnvironmentGuard envGuard{ m_Closure, interpreter };

		for (size_t i{}; i < arity(); i++)
		{
			interpreter.m_Environment->define(m_Declaration->m_Parameters[i].m_Lexeme, arguments[i]);
		}

		try {

			m_Declaration->m_Body->Accept(interpreter);

		}
		catch (const Interpreter::ReturnSignal& signal)
		{
			return signal.value;
		}

		return Nil{};
	}

	uint32_t FunctionObject::arity() const
	{
		return m_Declaration->m_Parameters.size();
	}

	std::string FunctionObject::ToString() const
	{
		return "<fucn " + m_Declaration->m_Name.m_Lexeme + ">";
	}
}