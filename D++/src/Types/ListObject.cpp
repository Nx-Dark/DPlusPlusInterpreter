#include "dpch.h"
#include "ListObject.h"


namespace DXX
{

	ListObject::ListObject(std::vector<Literal> elements)
		: m_Elements{ std::move(elements) } 
	{

	}

	ListObject::~ListObject()
	{

	}

}