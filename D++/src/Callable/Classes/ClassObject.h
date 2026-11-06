#pragma once

#include "Callable/Callable.h"

namespace DXX
{

	class FunctionObject;
	class Token;

	//this represents the class blue print;
	class ClassObject : public Callable, public std::enable_shared_from_this<ClassObject>
	{

	private:

		std::string m_Name{};
		std::unordered_map<std::string, Ref<Callable>> m_Methods{}; //the Callable is a FunctionObject;

		friend class ClassInstance;

	public:
		ClassObject(const std::string& name, std::unordered_map<std::string, Ref<Callable>> methods);
		virtual ~ClassObject() = default;
		
		//when Call is called, it should return a Ref<ClassInstance> which is the runtime object;
		virtual Literal Call(Interpreter& interpreter, const std::vector<Literal>& arguments) override;
		virtual uint32_t arity() const override;
		virtual std::string ToString() const;

		Ref<Callable> FindMethod(const Token& name) const;
		Ref<Callable> FindMethod(const std::string& name) const;
	};

}