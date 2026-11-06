#include "dpch.h"
#include "ClassInstance.h"

#include "Tokenizer/Token.h"
#include "Callable/Functions/FunctionObject.h"

namespace DXX
{

	ClassInstance::ClassInstance(Ref<ClassObject> classObj)
		: m_ClassObject{ classObj }
	{
	}

	std::optional<Literal> ClassInstance::Get(Token name)
	{
		auto field{ m_Fields.find(name.m_Lexeme) };

		if (field != m_Fields.end())
			return field->second;

		Ref<FunctionObject> method{ std::dynamic_pointer_cast<FunctionObject>(m_ClassObject->FindMethod(name)) };
		if (method)
			return method->Bind(shared_from_this());

		return std::nullopt;
	}

	bool ClassInstance::Set(Token name, const Literal& value)
	{
		m_Fields[name.m_Lexeme] = value;
		return true;
	}

	std::string ClassInstance::ToString() const
	{
		return "<class " + m_ClassObject->m_Name + " instance>";
	}

}