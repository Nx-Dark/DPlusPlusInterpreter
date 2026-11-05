#pragma once

#include "ClassObject.h"

namespace DXX
{

	class ClassInstance
	{
	private:

		Ref<ClassObject> m_ClassObject{};

		std::unordered_map<std::string, Literal> m_Fields{};

	public:
		ClassInstance(Ref<ClassObject> classObj);
		~ClassInstance() = default;

		std::optional<Literal> Get(Token name) const;
		void Set(Token name, const Literal& value);

		std::string ToString() const;
	};

}