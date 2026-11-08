#include "dpch.h"
#include "ListObject.h"

#include "Interpreter/Interpreter.h"

namespace DXX
{

	ListObject::ListObject(std::vector<Literal> elements)
		: m_Elements{ std::move(elements) } 
	{

	}

	ListObject::~ListObject()
	{

	}

	void ListObject::Push(const Literal& value)
	{
		m_Elements.push_back(value);
	}

	Literal ListObject::Pop(std::size_t index)
	{
		Literal value{ m_Elements[index] };
		m_Elements.erase(m_Elements.begin() + index);
		return value;
	}

	std::size_t ListObject::Length()
	{
		return m_Elements.size();
	}

	void ListObject::Clear()
	{
		m_Elements.clear();
	}

	bool ListObject::Contains(const Literal& value)
	{
		auto it{ std::find(m_Elements.begin(), m_Elements.end(), value) };
		if (it != m_Elements.end())
			return true;
		return false;
	}

	void ListObject::Insert(std::size_t index, const Literal& value)
	{
		m_Elements.insert(m_Elements.begin() + index, value);
	}

	bool ListObject::Remove(const Literal& value)
	{
		auto it{ std::find(m_Elements.begin(), m_Elements.end(), value) };
		if (it != m_Elements.end())
		{
			m_Elements.erase(it);
			return true;
		}

		return false;
	}

	int64_t ListObject::IndexOf(const Literal& value)
	{
		auto it{ std::find(m_Elements.begin(), m_Elements.end(), value) };
		int64_t index{ -1 };

		if (it != m_Elements.end())
			index = static_cast<int64_t>(it - m_Elements.begin());

		return index;
	}

	void ListObject::Reverse()
	{
		std::reverse(m_Elements.begin(), m_Elements.end());
	}

	//list methods
	Literal ListPush::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& value{ arguments[0] };
		m_ListObject->Push(value);
		return Nil{};
	}

	Literal ListPop::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const std::size_t listLength{ m_ListObject->Length() };

		if(listLength == 0)
			throw interpreter.nativeError("Cannot pop from an empty list.");

		if (!arguments.empty())
		{
			const Literal& value{ arguments[0] };
			double index{};

			if (std::holds_alternative<double>(value))
			{
				index = std::get<double>(value);
			}
			else
				throw interpreter.nativeError("Unsupported argument, pop takes in a number.");

			if (!std::isfinite(index))
				throw interpreter.nativeError("pop requires a finite Integer.");

			if (std::floor(index) != index)
				throw interpreter.nativeError("pop requires a Integer.");

			if (index < 0)
				throw interpreter.nativeError("pop requires a Positive Integer.");

			std::size_t intIndex{ static_cast<std::size_t>(index) };

			if (intIndex >= m_ListObject->Length())
				throw interpreter.nativeError("Index out of range.");

			return m_ListObject->Pop(intIndex);
		}

		return m_ListObject->Pop(listLength - 1);
	}

	Literal ListLength::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{	
		std::size_t length{ m_ListObject->Length() };
		return static_cast<double>(length);
	}

	Literal ListClear::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		m_ListObject->Clear();
		return Nil{};
	}

	Literal ListContains::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& value{ arguments[0] };
		return m_ListObject->Contains(value);
	}

	Literal ListInsert::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& indexLiteral{ arguments[0] };
		const Literal& insertValue{ arguments[1] };
		double index{};

		if (std::holds_alternative<double>(indexLiteral))
		{
			index = std::get<double>(indexLiteral);
		}
		else
			throw interpreter.nativeError("Unsupported argument, insert takes in a number in pos 0.");

		if(!std::isfinite(index))
			throw interpreter.nativeError("insert requires a finite Integer.");

		if (std::floor(index) != index)
			throw interpreter.nativeError("insert requires a Integer.");

		if (index < 0)
			throw interpreter.nativeError("insert requires a Positive Integer.");

		std::size_t intIndex{ static_cast<std::size_t>(index) };

		if (intIndex >= m_ListObject->Length())
		{
			m_ListObject->Insert(m_ListObject->Length(), insertValue);
			return Nil{};
		}

		m_ListObject->Insert(intIndex, insertValue);
		return Nil{};
	}

	Literal ListRemove::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& removeValue{ arguments[0] };

		if (!m_ListObject->Remove(removeValue))
			throw interpreter.nativeError("Element not found in the list to remove.");

		return Nil{};
	}

	Literal ListIndexOf::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& value{ arguments[0] };
		return static_cast<double>(m_ListObject->IndexOf(value));
	}

	Literal ListReverse::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		m_ListObject->Reverse();
		return Nil{};
	}

	Literal ListCopy::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		std::vector<Literal> elements{ m_ListObject->getElements() };
		return CreateRef<ListObject>(std::move(elements));
	}

	Literal ListExtend::Call(Interpreter& interpreter, const std::vector<Literal>& arguments)
	{
		const Literal& iterable{ arguments[0] };

		if (!(std::holds_alternative<Ref<ListObject>>(iterable)))
			throw interpreter.nativeError("Only lists are iteralble.");

		Ref<ListObject> listIterable{ std::get<Ref<ListObject>>(iterable) };

		for (const auto& element : listIterable->getElements())
			m_ListObject->Push(element);

		return Nil{};
	}

}