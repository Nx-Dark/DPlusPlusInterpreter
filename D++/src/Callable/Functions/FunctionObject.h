#pragma once

#include "Callable/Callable.h"

namespace DXX
{
	
	class Environment;
	class FunctionStmt;
	class Token;

	class FunctionObject : public Callable
	{
	private:

		Ref<FunctionStmt> m_Declaration{};
		Ref<Environment> m_Closure{};

		bool m_IsInitializer{}; //to see if this function is an initializer inside a class body;

	public:
		FunctionObject(Ref<FunctionStmt> decl, const Ref<Environment>& closure, bool initializer = false);
		virtual ~FunctionObject() = default;

		Ref<Callable> Bind(Ref<ClassInstance> classInstance);

		//override;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;

		virtual uint32_t minArity() const override;
		virtual uint32_t maxArity() const override;

		virtual std::string ToString() const override;

	};

}