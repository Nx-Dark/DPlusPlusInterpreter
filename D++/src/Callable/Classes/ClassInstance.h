#pragma once

#include "ClassObject.h"

namespace DXX
{

	class ClassInstance : public std::enable_shared_from_this<ClassInstance>
	{
	private:

		Ref<ClassObject> m_ClassObject{};

		std::unordered_map<std::string, Literal> m_Fields{};

	public:
		ClassInstance(Ref<ClassObject> classObj);
		~ClassInstance() = default;

		std::optional<Literal> Get(Token name);
		bool Set(Token name, const Literal& value);

		std::string ToString() const;
	};

}