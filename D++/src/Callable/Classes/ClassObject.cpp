#include "dpch.h"
#include "ClassObject.h"

#include "ClassInstance.h"
#include "Callable/Functions/FunctionObject.h"
#include "Tokenizer/Token.h"

namespace DXX
{

	ClassObject::ClassObject(const std::string& name, std::unordered_map<std::string, Ref<Callable>> methods)
		: m_Name{ name }, m_Methods{ std::move(methods) }
	{
	}

	Literal ClassObject::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		auto instance{ CreateRef<ClassInstance>(shared_from_this()) };
		return instance;
	}

	uint32_t ClassObject::arity() const
	{
		return 0u;
	}

	std::string ClassObject::ToString() const
	{
		return "<class " + m_Name + ">";
	}

	Ref<Callable> ClassObject::FindMethod(const Token& name) const 
	{
		auto it{ m_Methods.find(name.m_Lexeme) };

		if (it != m_Methods.end())
			return it->second;

		return nullptr;
	}

}