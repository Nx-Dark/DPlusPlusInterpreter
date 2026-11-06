#include "dpch.h"
#include "ClassObject.h"

#include "ClassInstance.h"
#include "Callable/Functions/FunctionObject.h"
#include "Tokenizer/Token.h"

namespace DXX
{

	ClassObject::ClassObject(const std::string& name, Ref<ClassObject> superClass, std::unordered_map<std::string, Ref<Callable>> methods)
		: m_Name{ name }, m_SuperClass{ superClass }, m_Methods{ std::move(methods) }
	{
	}

	Literal ClassObject::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		Ref<ClassInstance> instance{ CreateRef<ClassInstance>(shared_from_this()) };

		//initializer
		Ref<FunctionObject> initializerMethod{ std::dynamic_pointer_cast<FunctionObject>(FindMethod("construct")) };
		if (initializerMethod)
			initializerMethod->Bind(instance)->Call(interpreter, arguments);

		return instance;
	}

	uint32_t ClassObject::arity() const
	{
		Ref<Callable> initializerMethod{ FindMethod("construct") };
		if (!initializerMethod) return 0;
		return initializerMethod->arity();
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

		if (m_SuperClass)
			return m_SuperClass->FindMethod(name.m_Lexeme);

		return nullptr;
	}

	Ref<Callable> ClassObject::FindMethod(const std::string& name) const
	{
		auto it{ m_Methods.find(name) };

		if (it != m_Methods.end())
			return it->second;

		return nullptr;
	}

}