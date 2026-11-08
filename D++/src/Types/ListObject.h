#pragma once

#include "Literal.h"
#include "Callable/Callable.h"

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
		
		inline const std::vector<Literal>& getElements() const { return m_Elements; }

		void Push(const Literal& value);
		Literal Pop(std::size_t index);
		std::size_t Length();
		void Clear();
		bool Contains(const Literal& value);
		void Insert(std::size_t index, const Literal& value);
		bool Remove(const Literal& value);
		int64_t IndexOf(const Literal& value);
		void Reverse();

		inline std::string ToString() const { return "<list>"; }
	};

	//list functions exposed via callable;
	class ListPush : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListPush(Ref<ListObject> listObject)
			: m_ListObject{ listObject } 
		{
		}
		virtual ~ListPush() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListPop : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListPop(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListPop() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListLength : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListLength(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListLength() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListClear : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListClear(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListClear() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListContains : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListContains(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListContains() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListInsert : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListInsert(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListInsert() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListRemove : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListRemove(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListRemove() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListIndexOf : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListIndexOf(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListIndexOf() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListReverse : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListReverse(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListReverse() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListCopy : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListCopy(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListCopy() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

	class ListExtend : public Callable
	{
		Ref<ListObject> m_ListObject{};

	public:
		ListExtend(Ref<ListObject> listObject)
			: m_ListObject{ listObject }
		{
		}
		virtual ~ListExtend() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native List method>"; }
	};

}