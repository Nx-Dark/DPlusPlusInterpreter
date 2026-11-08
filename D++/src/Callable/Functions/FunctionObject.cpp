#include "dpch.h"
#include "FunctionObject.h"
#include "Callable/Classes/ClassInstance.h"

#include "AST/Statement.h"
#include "Interpreter/Environment.h"
#include "Interpreter/Interpreter.h"

namespace DXX
{

	FunctionObject::FunctionObject(Ref<FunctionStmt> decl, const Ref<Environment>& closure, bool initializer)
		: m_Declaration{ decl }, m_Closure{ closure }, m_IsInitializer{ initializer }
	{
	}

	Ref<Callable> FunctionObject::Bind(Ref<ClassInstance> classInstance)
	{
		Ref<Environment> env{ CreateRef<Environment>(m_Closure) };
		env->define("this", classInstance);
		return CreateRef<FunctionObject>(m_Declaration, env, m_IsInitializer);
	}

	Literal FunctionObject::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		EnvironmentGuard envGuard{ m_Closure, interpreter };

		for (size_t i{}; i < maxArity(); i++)
		{
			interpreter.m_Environment->define(m_Declaration->m_Parameters[i].m_Lexeme, arguments[i]);
		}

		try {

			m_Declaration->m_Body->Accept(interpreter);

		}
		catch (const Interpreter::ReturnSignal& signal)
		{
			if (m_IsInitializer)
				return m_Closure->getAt(0, "this").value();

			return signal.value;
		}

		if (m_IsInitializer)
			return m_Closure->getAt(0, "this").value();

		return Nil{};
	}

	uint32_t FunctionObject::minArity() const
	{
		return m_Declaration->m_Parameters.size();
	}

	uint32_t FunctionObject::maxArity() const
	{
		return m_Declaration->m_Parameters.size();
	}

	std::string FunctionObject::ToString() const
	{
		return "<func " + m_Declaration->m_Name.m_Lexeme + ">";
	}
}