#pragma once

#include "Types/Literal.h"
#include "Core/Core.h"

namespace DXX
{

	class Environment
	{

	private:

		std::unordered_map<std::string, Literal> m_Values{};

		Ref<Environment> m_Enclosing{ nullptr };

	public:
		Environment() = default;
		Environment(Ref<Environment> enclosing);
		~Environment() = default;

		//getters
		void define(const std::string& name, Literal value);
		std::optional<Literal> get(const std::string& name) const;
		bool assign(const std::string& name, Literal value);

		std::optional<Literal> getAt(int distance, const std::string& name) const;
		bool assignAt(int distance, const std::string& name, const Literal& value);

	private:

		Ref<Environment> getAncestor(int distance) const;

	};

	class Interpreter;

	//environment guard to restore to previous env if runtime error gets thrown;
	class EnvironmentGuard
	{
	private:
		Ref<Environment> m_Previous{};
		Interpreter& m_Interpreter;

	public:
		EnvironmentGuard(Ref<Environment> previous, Interpreter& interpreter);

		~EnvironmentGuard();

		EnvironmentGuard(const EnvironmentGuard&) = delete;
		EnvironmentGuard& operator=(const EnvironmentGuard&) = delete;
	};

}