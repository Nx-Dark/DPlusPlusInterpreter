#include "dpch.h"
#include "Environment.h"

namespace DXX
{

	Environment::Environment(Ref<Environment> enclosing)
		: m_Enclosing{ enclosing } 
	{
	}

	void Environment::define(const std::string& name, Literal value)
	{
		m_Values[name] = std::move(value);
	}

	std::optional<Literal> Environment::get(const std::string& name) const
	{
		auto it{ m_Values.find(name) };

		if (it != m_Values.end())
			return it->second;

		else if (m_Enclosing)
			return m_Enclosing->get(name);

		return std::nullopt;
	}

	bool Environment::assign(const std::string& name, Literal value)
	{
		auto it{ m_Values.find(name) };

		if (it != m_Values.end())
		{
			it->second = value;
			return true;
		}

		else if (m_Enclosing)
			return m_Enclosing->assign(name, std::move(value));

		return false;
	}
}