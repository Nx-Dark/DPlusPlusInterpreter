#pragma once

#include "Callable.h"

namespace DXX
{
	
	class Environment;
	class FunctionStmt;

	class FunctionObject : public Callable
	{
	private:

		Ref<FunctionStmt> m_Declaration{};
		Ref<Environment> m_Closure{};

	public:
		FunctionObject(const Ref<FunctionStmt>& decl, const Ref<Environment>& closure);
		virtual ~FunctionObject() = default;

		//override;
		Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;

		uint32_t arity() const override;

		std::string ToString() const override;

	};

}