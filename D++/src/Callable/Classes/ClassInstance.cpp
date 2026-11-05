#include "dpch.h"
#include "ClassInstance.h"

#include "Tokenizer/Token.h"

namespace DXX
{

	ClassInstance::ClassInstance(Ref<ClassObject> classObj)
		: m_ClassObject{ classObj }
	{
	}

	std::optional<Literal> ClassInstance::Get(Token name) const
	{
		auto field{ m_Fields.find(name.m_Lexeme) };

		if (field != m_Fields.end())
			return field->second;

		Ref<Callable> method{ m_ClassObject->FindMethod(name) };
		if (method)
			return method;

		return std::nullopt;
	}

	void ClassInstance::Set(Token name, const Literal& value)
	{
		m_Fields[name.m_Lexeme] = value;
	}

	std::string ClassInstance::ToString() const
	{
		return "<class " + m_ClassObject->m_Name + " instance>";
	}

}