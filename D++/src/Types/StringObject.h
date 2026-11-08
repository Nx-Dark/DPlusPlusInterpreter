#pragma once

#include "Callable/Callable.h"

namespace DXX
{

	class StringLength : public Callable
	{
		std::string m_String{};
	public:
		StringLength(const std::string& string)
			: m_String{ string } 
		{
		}
		virtual ~StringLength() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringIsEmtpy : public Callable
	{
		std::string m_String{};
	public:
		StringIsEmtpy(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringIsEmtpy() = default;
		inline virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override { return m_String.empty(); }
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringUpper : public Callable
	{
		std::string m_String{};
	public:
		StringUpper(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringUpper() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringLower : public Callable
	{
		std::string m_String{};
	public:
		StringLower(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringLower() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringContains : public Callable
	{
		std::string m_String{};
	public:
		StringContains(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringContains() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringStartsWith : public Callable
	{
		std::string m_String{};
	public:
		StringStartsWith(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringStartsWith() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringEndsWith : public Callable
	{
		std::string m_String{};
	public:
		StringEndsWith(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringEndsWith() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringIndexOf : public Callable
	{
		std::string m_String{};
	public:
		StringIndexOf(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringIndexOf() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringReplace : public Callable
	{
		std::string m_String{};
	public:
		StringReplace(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringReplace() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringTrim : public Callable
	{
		std::string m_String{};
	public:
		StringTrim(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringTrim() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringReverse : public Callable
	{
		std::string m_String{};
	public:
		StringReverse(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringReverse() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 0; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

	class StringSubstring : public Callable
	{
		std::string m_String{};
	public:
		StringSubstring(const std::string& string)
			: m_String{ string }
		{
		}
		virtual ~StringSubstring() = default;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native str method>"; }
	};

}