#pragma once

#include "Tokenizer/Literal.h"
#include "Core/Core.h"

namespace DXX
{

	class Environment
	{

	private:

		std::unordered_map<std::string, Literal> m_Values{};

	public:
		Environment() = default;
		~Environment() = default;

		//getters
		void define(const std::string& name, Literal value);
		std::optional<Literal> get(const std::string& name) const;
		bool assign(const std::string& name, Literal value);

	};

}