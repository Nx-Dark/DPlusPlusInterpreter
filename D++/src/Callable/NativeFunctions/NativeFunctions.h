#pragma once

#include "Callable/Callable.h"

namespace DXX
{

	class ClockFunction : public Callable
	{

	public:
		virtual ~ClockFunction() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;

		inline virtual uint32_t arity() const override { return 0; }

		inline virtual std::string ToString() const override { return "<native clock func>"; }

	};

	class PrintFunction : public Callable
	{
	public:
		virtual ~PrintFunction() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return VARIADIC_ARITY; }
		inline virtual std::string ToString() const override { return "<native println func>"; }
	};

	class SqrtFunc : public Callable
	{
	public:
		virtual ~SqrtFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native sqrt func>"; }
	};

	class PowFunc : public Callable
	{
	public:
		virtual ~PowFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native pow func>"; }
	};

	class MaxFunc : public Callable
	{
	public:
		virtual ~MaxFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native max func>"; }
	};

	class MinFunc : public Callable
	{
	public:
		virtual ~MinFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<native min func>"; }
	};

	class InputFunc : public Callable
	{
	public:
		virtual ~InputFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native input func>"; }
	};

	//casting functions
	class StrCastFunc : public Callable
	{
	public:
		virtual ~StrCastFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native str func>"; }

	};

	class DoubleCastFunc : public Callable
	{
	public:
		virtual ~DoubleCastFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native double func>"; }

	};

	class BoolCastFunc : public Callable
	{
	public:
		virtual ~BoolCastFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native bool func>"; }
	};

	class TypeCheckFunc : public Callable
	{
	public:
		virtual ~TypeCheckFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<native typeof func>"; }
	};


}