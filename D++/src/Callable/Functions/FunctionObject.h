#pragma once

#include "Callable/Callable.h"

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
		FunctionObject(Ref<FunctionStmt> decl, const Ref<Environment>& closure);
		virtual ~FunctionObject() = default;

		Ref<Callable> Bind(Ref<ClassInstance> classInstance);

		//override;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;

		virtual uint32_t arity() const override;

		virtual std::string ToString() const override;

	};

}