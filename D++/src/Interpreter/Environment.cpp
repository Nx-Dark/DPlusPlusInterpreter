#include "dpch.h"
#include "Environment.h"

namespace DXX
{

	void Environment::define(const std::string& name, Literal value)
	{
		m_Values[name] = std::move(value);
	}

	std::optional<Literal> Environment::get(const std::string& name) const
	{
		auto it{ m_Values.find(name) };

		if (it != m_Values.end())
			return it->second;

		return std::nullopt;
	}

	bool Environment::assign(const std::string& name, Literal value)
	{
		if (m_Values.contains(name))
		{
			m_Values[name] = value;
			return true;
		}

		return false;
	}
}