#pragma once

#include "Types/Literal.h"
#include "Core/Core.h"

namespace DXX
{

	class Environment : public std::enable_shared_from_this<Environment>
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

		std::optional<Literal> getAt(int distance, const std::string& name) ;
		bool assignAt(int distance, const std::string& name, const Literal& value);

	private:

		Ref<Environment> getAncestor(int distance);

	};

	class Interpreter;

	//environment guard to restore to previous env if runtime error gets thrown;
	class EnvironmentGuard
	{
	private:
		Interpreter& m_Interpreter;
		Ref<Environment> m_Previous{};
		Ref<Environment> m_Current{};

	public:
		EnvironmentGuard(Ref<Environment> previous, Interpreter& interpreter);

		~EnvironmentGuard();

		inline Ref<Environment> GetCurrent() const { return m_Current; }

		EnvironmentGuard(const EnvironmentGuard&) = delete;
		EnvironmentGuard& operator=(const EnvironmentGuard&) = delete;
	};

}