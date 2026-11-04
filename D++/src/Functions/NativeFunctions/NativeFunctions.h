#pragma once

#include "Functions/Callable.h"

namespace DXX
{

	class ClockFunction : public Callable
	{

	public:
		virtual ~ClockFunction() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;

		inline virtual uint32_t arity() const override { return 0; }

		inline virtual std::string ToString() const override { return "<native func>"; }

	};

	class PrintFunction : public Callable
	{
	public:
		virtual ~PrintFunction() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return VARIADIC_ARITY; }
		inline virtual std::string ToString() const override { return "<nativ func>"; }
	};

	class SqrtFunc : public Callable
	{
	public:
		virtual ~SqrtFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 1; }
		inline virtual std::string ToString() const override { return "<nativ func>"; }
	};

	class PowFunc : public Callable
	{
	public:
		virtual ~PowFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<nativ func>"; }
	};

	class MaxFunc : public Callable
	{
	public:
		virtual ~MaxFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<nativ func>"; }
	};

	class MinFunc : public Callable
	{
	public:
		virtual ~MinFunc() = default;

		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		inline virtual uint32_t arity() const override { return 2; }
		inline virtual std::string ToString() const override { return "<nativ func>"; }
	};


}