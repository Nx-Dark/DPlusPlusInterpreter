#include "dpch.h"
#include "Environment.h"

#include "Interpreter.h"

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

	std::optional<Literal> Environment::getAt(int distance, const std::string& name)
	{
		Ref<Environment> env{ getAncestor(distance) };

		auto it{ env->m_Values.find(name) };

		if (it != env->m_Values.end())
			return it->second;

		return std::nullopt;
	}

	bool Environment::assignAt(int distance, const std::string& name, const Literal& value)
	{
		Ref<Environment> env{ getAncestor(distance) };

		if (env->m_Values.contains(name))
		{
			env->m_Values[name] = value;
			return true;
		}
		
		return false;
	}

	Ref<Environment> Environment::getAncestor(int distance)
	{
		Ref<Environment> env{ shared_from_this() };

		for (int i{}; i < distance; i++)
			env = env->m_Enclosing;

		return env;
	}

	//environment guard;
	EnvironmentGuard::EnvironmentGuard(Ref<Environment> previous, Interpreter& interpreter)
		: m_Interpreter{ interpreter }, m_Previous{ interpreter.m_Environment }
	{
		m_Current = CreateRef<Environment>(previous);
		m_Interpreter.m_Environment = m_Current;
	}

	EnvironmentGuard::~EnvironmentGuard()
	{
		m_Interpreter.m_Environment = m_Previous;
	}
}