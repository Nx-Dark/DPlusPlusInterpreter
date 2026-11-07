#pragma once

#include "Literal.h"

namespace DXX
{

	class ListObject
	{
	private:
		std::vector<Literal> m_Elements{};

		friend class Interpreter;
	public:
		ListObject(std::vector<Literal> elements);
		~ListObject();
	};

}