#include "dpch.h"
#include "Interpreter.h"

#include "AST/Expression.h"
#include "AST/Statement.h"

#include "Callable/Functions/FunctionObject.h"
#include "Callable/NativeFunctions/NativeFunctions.h"
#include "Callable/Classes/ClassObject.h"
#include "Callable/Classes/ClassInstance.h"
#include "Types/ListObject.h"
#include "Types/StringObject.h"

namespace DXX
{

	static bool isTruthy(const Literal& literal)
	{
		if (std::holds_alternative<Nil>(literal)) return false;
		else if (std::holds_alternative<bool>(literal)) return std::get<bool>(literal);
		else if (std::holds_alternative<float>(literal) && std::get<float>(literal) == 0.0f) return false;
		else if (std::holds_alternative<double>(literal) && std::get<double>(literal) == 0.0f) return false;
		else return true;
	}

	Interpreter::Interpreter(DiagnosticEngine& engine)
		: m_DiagnosticEngine{ engine }, m_GlobalEnv{ CreateRef<Environment>() }, m_Environment{ m_GlobalEnv }
	{
		DefineNativeFunctions();
	}

	void Interpreter::Interpret(const Expr& expr)
	{
		try {
			Literal literal{ expr.Accept(*this) };
			std::println("{0}", LiteralToString(literal));
		}
		catch (const RuntimeError&)
		{
			return;
		}
	}

	void Interpreter::Interpret(std::vector<Ref<Stmt>> stmts)
	{
		try {
			for (const auto& stmt : stmts)
			{
				stmt->Accept(*this);
			}
		}
		catch (const RuntimeError&)
		{
			return;
		}
	}

	Literal Interpreter::visitLiteral(const LiteralExpr& literalExpr)
	{
		return literalExpr.m_Literal;
	}

	Literal Interpreter::visitUnary(const UnaryExpr& unaryExpr)
	{
		Literal operand{ unaryExpr.m_Expr->Accept(*this) };
		const Token& op{ unaryExpr.m_Operator };

		switch (op.m_Type)
		{
			case TokenType::MINUS: {
				if (std::holds_alternative<float>(operand))
					return -(std::get<float>(operand));
				throw error(op, "Operation must done on a number.");
			}
			case TokenType::BANG:
			{
				if (std::holds_alternative<Nil>(operand))
					return true;
				else if (std::holds_alternative<bool>(operand))
					return !(std::get<bool>(operand));
				else
					return false;
			}
			default:
				throw error(op, "Unexpected Error Occured.");
		}

		return Nil{};
	}

	Literal Interpreter::visitBinary(const BinaryExpr& binaryExpr)
	{
		const Token& op{ binaryExpr.m_Operator };
		Literal leftOperand{ binaryExpr.m_LeftExpr->Accept(*this) };
		Literal rightOperand{ binaryExpr.m_RightExpr->Accept(*this) };

		return doBinaryOperation(leftOperand, op, rightOperand);
	}

	Literal Interpreter::visitGrouping(const GroupingExpr& groupingExpr)
	{
		return groupingExpr.m_Expr->Accept(*this);
	}

	Literal Interpreter::visitVariable(const VariableExpr& variableExpr)
	{
		return lookUpVariable(variableExpr.m_Token, variableExpr);
	}

	Literal Interpreter::visitAssignment(const AssignExpr& assignExpr)
	{
		Literal value{ assignExpr.m_Expr->Accept(*this) };

		auto it{ m_Locals.find(assignExpr.GetID()) };
		bool assigned{};

		if (it != m_Locals.end())
		{
			int distance{ it->second };
			assigned = m_Environment->assignAt(distance, assignExpr.m_Identifier.m_Lexeme, value);
		}

		else
		{
			assigned = m_GlobalEnv->assign(assignExpr.m_Identifier.m_Lexeme, value);
		}

		if (!assigned) throw error(assignExpr.m_Identifier, "Undefine Variable " + assignExpr.m_Identifier.m_Lexeme + ".");

		return value;
	}

	Literal Interpreter::visitCall(const CallExpr& callExpr)
	{
		Literal callee{ callExpr.m_Callee->Accept(*this) };

		std::vector<Literal> arguments{};

		for (auto& args : callExpr.m_Arguments)
		{
			arguments.push_back(args->Accept(*this));
		}

		auto callableObject{ std::get_if<Ref<Callable>>(&callee) };

		if (callableObject)
		{
			size_t callableObjArity{ (*callableObject)->arity() };
			size_t calleeArgCount{ callExpr.m_Arguments.size() };

			if (callableObjArity != VARIADIC_ARITY && callableObjArity != calleeArgCount)
			{
				throw error(callExpr.m_ClosingParen, "Expected " + std::to_string(callableObjArity) + " Arguments, but got " + std::to_string(calleeArgCount) + " Instead");
			}

			try 
			{
				return (*callableObject)->Call(*this, arguments);
			}

			catch (const NativeRuntimeError& nre)
			{
				throw error(callExpr.m_ClosingParen, nre.msg);
			}
		}

		throw error(callExpr.m_ClosingParen, "Only Callables are Allowed to be Called.");
	}

	Literal Interpreter::visitGet(const GetExpr& getExpr)
	{
		Literal object{ getExpr.m_Object->Accept(*this) };

		auto classInstanceObj{ std::get_if<Ref<ClassInstance>>(&object) };
		auto listObjectPtr{ std::get_if<Ref<ListObject>>(&object) };
		auto strObjectPtr{ std::get_if<std::string>(&object) };

		std::optional<Literal> propertyVal{};

		if (classInstanceObj)
			propertyVal = (*classInstanceObj)->Get(getExpr.m_Identifier);
		else if (listObjectPtr)
		{
			Ref<ListObject> listObject{ *listObjectPtr };
			
			const std::string& methodName{ getExpr.m_Identifier.m_Lexeme };

			if (methodName == "push")
				return CreateRef<ListPush>(listObject);
			else if (methodName == "pop")
				return CreateRef<ListPop>(listObject);
			else if (methodName == "length")
				return CreateRef<ListLength>(listObject);
			else if (methodName == "clear")
				return CreateRef<ListClear>(listObject);
			else if (methodName == "contains")
				return CreateRef<ListContains>(listObject);
			else if (methodName == "insert")
				return CreateRef<ListInsert>(listObject);
			else if (methodName == "remove")
				return CreateRef<ListRemove>(listObject);
			else if (methodName == "indexOf")
				return CreateRef<ListIndexOf>(listObject);
			else if (methodName == "reverse")
				return CreateRef<ListReverse>(listObject);
			else if (methodName == "copy")
				return CreateRef<ListCopy>(listObject);
			else if (methodName == "extend")
				return CreateRef<ListExtend>(listObject);

			throw error(getExpr.m_Identifier, "Undefined list property/method " + getExpr.m_Identifier.m_Lexeme + ".");
		}
		else if (strObjectPtr)
		{
			const std::string& strObject{ *strObjectPtr };

			const std::string& methodName{ getExpr.m_Identifier.m_Lexeme };
			
			if (methodName == "length")
				return CreateRef<StringLength>(strObject);
			else if (methodName == "upper")
				return CreateRef<StringUpper>(strObject);
			else if (methodName == "lower")
				return CreateRef<StringLower>(strObject);
			else if (methodName == "contains")
				return CreateRef<StringContains>(strObject);
			else if (methodName == "startsWith")
				return CreateRef<StringStartsWith>(strObject);
			else if (methodName == "endsWith")
				return CreateRef<StringEndsWith>(strObject);
			else if (methodName == "indexOf")
				return CreateRef<StringIndexOf>(strObject);
			else if (methodName == "replace")
				return CreateRef<StringReplace>(strObject);
			else if (methodName == "trim")
				return CreateRef<StringTrim>(strObject);

			throw error(getExpr.m_Identifier, "Undefined str property/method " + getExpr.m_Identifier.m_Lexeme + ".");
		}

		else 
			throw error(getExpr.m_Identifier, "Properties can only be accesses on Instances of Lists or Classes");
		
		if (propertyVal)
			return propertyVal.value();

		throw error(getExpr.m_Identifier, " Undefined property/field named " + getExpr.m_Identifier.m_Lexeme + ".");
	}

	Literal Interpreter::visitSet(const SetExpr& setExpr)
	{
		Literal object{ setExpr.m_Object->Accept(*this) };
		Token op{ setExpr.m_Operator };

		auto classInstanceObj{ std::get_if<Ref<ClassInstance>>(&object) };

		if (classInstanceObj)
		{
			Literal value{ setExpr.m_Value->Accept(*this) };
			Literal assignValue{ Nil{} };
			if (op.m_Type == TokenType::EQUAL)
			{
				assignValue = value;
			}
			else
			{
				auto classInstanceValue{ (*classInstanceObj)->Get(setExpr.m_Identifier) };
				if (classInstanceValue)
					assignValue = doBinaryOperation(classInstanceValue.value(), op, value);
				else
					throw error(setExpr.m_Identifier, (*classInstanceObj)->ToString() + " Undefined property/field named " + setExpr.m_Identifier.m_Lexeme + ".");
			}
			bool assigned{ (*classInstanceObj)->Set(setExpr.m_Identifier, assignValue) };
			if (!assigned) throw error(setExpr.m_Identifier, (*classInstanceObj)->ToString() + " Undefined property/field named " + setExpr.m_Identifier.m_Lexeme + ".");
			return value;
		}

		throw error(setExpr.m_Identifier, "Only Instances have fields.");
	}

	Literal Interpreter::visitThis(const ThisExpr& thisExpr)
	{
		return lookUpVariable(thisExpr.m_Keyword, thisExpr);
	}

	Literal Interpreter::visitSuper(const SuperExpr& superExpr)
	{
		auto it{ m_Locals.find(superExpr.GetID()) };

		int distance{};

		if (it != m_Locals.end())
			distance = it->second;

		auto superClassLiteral{ m_Environment->getAt(distance, "super") };
		auto objectLiteral{ m_Environment->getAt(distance - 1, "this") };

		if (superClassLiteral)
		{
			auto superClassPtr{ std::get_if<Ref<Callable>>(&superClassLiteral.value()) };

			if (superClassPtr)
			{
				Ref<ClassObject> superClass{ std::dynamic_pointer_cast<ClassObject>(*superClassPtr) };

				if (superClass && objectLiteral)
				{
					auto objectPtr { std::get_if<Ref<ClassInstance>>(&objectLiteral.value()) };

					if (objectPtr)
					{
						Ref<ClassInstance> objectInstance{ *objectPtr };

						Ref<FunctionObject> method{ std::dynamic_pointer_cast<FunctionObject>(superClass->FindMethod(superExpr.m_Method.m_Lexeme)) };
						if (!method)
							throw error(superExpr.m_Method, "Undefined Property '" + superExpr.m_Method.m_Lexeme + "'.");

						return method->Bind(objectInstance);
					} 
				}
			}
		}
	}

	Literal Interpreter::visitList(const ListExpr& listExpr)
	{
		std::vector<Literal> elements{};

		for (const auto& element : listExpr.m_Elements)
		{
			elements.push_back(element->Accept(*this));
		}

		return CreateRef<ListObject>(elements);
	}

	Literal Interpreter::visitIndex(const IndexExpr& indexExpr)
	{
		Literal objectLiteral{ indexExpr.m_Object->Accept(*this) };
		Literal indexLiteral{ indexExpr.m_Index->Accept(*this) };

		auto listObjectPtr{ std::get_if<Ref<ListObject>>(&objectLiteral) };
		auto strObjectPtr{ std::get_if<std::string>(&objectLiteral) };

		if (listObjectPtr)
		{
			Ref<ListObject> listObject{ *listObjectPtr };

			double index{ validateIndex(indexExpr.m_keyword, indexLiteral) };

			std::size_t intIndex{ static_cast<std::size_t>(index) };

			if (intIndex >= listObject->m_Elements.size()) 
				throw error(indexExpr.m_keyword, "Index out of range.");

			return listObject->m_Elements[intIndex];
		}
		else if (strObjectPtr)
		{
			const std::string_view strObject{ *strObjectPtr };

			double index{ validateIndex(indexExpr.m_keyword, indexLiteral) };

			std::size_t intIndex{ static_cast<std::size_t>(index) };

			if (intIndex >= strObject.size())
				throw error(indexExpr.m_keyword, "Index out of range.");

			return std::string{ strObject[intIndex] };
		}
		throw error(indexExpr.m_keyword, "Cannot Index types other than list and str.");

	}

	Literal Interpreter::visitIndexSet(const IndexSetExpr& indexSetExpr)
	{
		Literal objectLiteral{ indexSetExpr.m_Object->Accept(*this) };
		Literal indexLiteral{ indexSetExpr.m_Index->Accept(*this) };
		Literal valueLiteral{ indexSetExpr.m_Value->Accept(*this) };
		const Token& op{ indexSetExpr.m_Operator };

		auto listObjectPtr{ std::get_if<Ref<ListObject>>(&objectLiteral) };
		if (listObjectPtr)
		{
			Ref<ListObject> listObject{ *listObjectPtr };

			double listIndex{ validateIndex(indexSetExpr.m_Operator, indexLiteral) };
			std::size_t intListIndex{ static_cast<std::size_t>(listIndex) };

			if (intListIndex >= listObject->m_Elements.size())
				throw error(op, "Index out of range.");

			Literal& currentValue{ listObject->m_Elements[intListIndex] };

			if (op.m_Type == TokenType::EQUAL)
			{
				currentValue = valueLiteral;
			}
			else {
				currentValue = doBinaryOperation(currentValue, op, valueLiteral);
			}

			return currentValue;
		}

		throw error(op, "Cannot Assign Index types other than list");

		return Nil{};
	}

	//------------------------------------------------------------------
	//visiting statements;

	void Interpreter::visitExpressionStmt(ExpressionStmt& expressionStmt)
	{
		expressionStmt.m_Expr->Accept(*this);
	}

	void Interpreter::visitPrintStmt(PrintStmt& printStmt)
	{

		Literal literal{ printStmt.m_Expr->Accept(*this) };

		std::println("{0}", LiteralToString(literal));
	}

	void Interpreter::visitVarStmt(VarStmt& varStmt)
	{
		Literal value{ Nil{} };

		if (varStmt.m_InitializerExpr)
			value = varStmt.m_InitializerExpr->Accept(*this);

		m_Environment->define(varStmt.m_Token.m_Lexeme, value);
	}

	void Interpreter::visitBlockStmt(BlockStmt& blockStmt)
	{

		EnvironmentGuard envGuard{ m_Environment, *this };

		for (auto& stmt : blockStmt.m_Statements)
			stmt->Accept(*this);

	}

	void Interpreter::visitIfStmt(IfStmt& ifStmt)
	{

		if (isTruthy(ifStmt.m_Condition->Accept(*this)))
			ifStmt.m_ThenBranch->Accept(*this);

		else if (ifStmt.m_ElseBranch)
			ifStmt.m_ElseBranch->Accept(*this);
	}

	void Interpreter::visitWhileStmt(WhileStmt& whileStmt)
	{

		EnvironmentGuard envGuard{ m_Environment, *this };

		while (isTruthy(whileStmt.m_Condition->Accept(*this)))
		{
			try {
				whileStmt.m_ThenBranch->Accept(*this);
			}

			catch (const BreakSignal&) {
				break;
			}

			catch (const ContinueSignal&) {
				continue;
			}
		}
	}

	void Interpreter::visitForStmt(ForStmt& forStmt)
	{

		EnvironmentGuard envGuard{ m_Environment, *this };

		if (forStmt.m_Initializer) forStmt.m_Initializer->Accept(*this);

		while (true)
		{
			//checking the condition
			if (forStmt.m_Condition) {
				if (!isTruthy(forStmt.m_Condition->Accept(*this)))
					break;
			}

			try {

				forStmt.m_Body->Accept(*this);
			}

			catch (const BreakSignal&) {
				break;
			}

			catch (const ContinueSignal&) {

			}

			//checking the increment
			if (forStmt.m_Increment)
				forStmt.m_Increment->Accept(*this);
		}
	}

	void Interpreter::visitBreakStmt(BreakStmt&)
	{
		throw BreakSignal{};
	}

	void Interpreter::visitContinueStmt(ContinueStmt&)
	{
		throw ContinueSignal{};
	}

	void Interpreter::visitReturnStmt(ReturnStmt& returnStmt)
	{
		Literal returnVal{ Nil{} };
		if (returnStmt.m_Expr)
			returnVal = returnStmt.m_Expr->Accept(*this);

		throw ReturnSignal{ returnVal };
	}

	void Interpreter::visitFunctionStmt(FunctionStmt& functionStmt)
	{
		auto fName{ functionStmt.m_Name };

		auto refFuncStatement{ CreateRef<FunctionStmt>(std::move(functionStmt.m_Name), functionStmt.m_Body, std::move(functionStmt.m_Parameters)) };

		m_Environment->define(fName.m_Lexeme, 
			CreateRef<FunctionObject>(refFuncStatement, m_Environment)
		);
	}

	void Interpreter::visitClassStmt(ClassStmt& classStmt)
	{

		Literal superClassLiteral{ Nil{} };
		Ref<ClassObject> superClass{ nullptr };

		if (classStmt.m_SuperClass)
		{
			superClassLiteral = classStmt.m_SuperClass->Accept(*this);
			auto callable{ std::get_if<Ref<Callable>>(&superClassLiteral) };
			if (!callable)
				throw error(classStmt.m_SuperClass->m_Token, "Superclass must be class.");

			superClass = std::dynamic_pointer_cast<ClassObject>(*callable);

			if(!superClass)
				throw error(classStmt.m_SuperClass->m_Token, "Superclass must be class.");
		}

		m_Environment->define(classStmt.m_Name.m_Lexeme, Nil{});

		if (superClass)
		{
			m_Environment = CreateRef<Environment>(m_Environment);
			m_Environment->define("super", superClass);
		}

		std::unordered_map<std::string, Ref<Callable>> methods{};

		for (auto& method : classStmt.m_Methods)
		{
			Ref<FunctionStmt> methodFuncStmt{ std::dynamic_pointer_cast<FunctionStmt>(method) };

			if (methodFuncStmt)
			{
				Ref<Callable> methodFuncObj{ CreateRef<FunctionObject>(methodFuncStmt, m_Environment, methodFuncStmt->m_Name.m_Lexeme == "construct")};
				methods[methodFuncStmt->m_Name.m_Lexeme] = methodFuncObj;
			}
		}

		Ref<Callable> classObj{ CreateRef<ClassObject>(classStmt.m_Name.m_Lexeme, superClass, std::move(methods)) };

		if (superClass)
			m_Environment = m_Environment->m_Enclosing;

		m_Environment->assign(classStmt.m_Name.m_Lexeme, classObj);
	}
	
	void Interpreter::Resolve(const Expr& expr, int depth)
	{
		m_Locals[expr.GetID()] = depth;
	}

	Literal Interpreter::lookUpVariable(Token token, const Expr& expr)
	{

		auto it{ m_Locals.find(expr.GetID()) };

		if (it != m_Locals.end())
		{
			int distance{ it->second };
			return m_Environment->getAt(distance, token.m_Lexeme).value();
		}

		auto value{ m_GlobalEnv->get(token.m_Lexeme) };

		if (!value)
			throw error(token, "Undefined Variable " + token.m_Lexeme + ".");

		return value.value();
	}

	Literal Interpreter::doBinaryOperation(const Literal& leftOperand, const Token& op, const Literal& rightOperand)
	{
		//for evalutaing 'and' and 'or' logical operators; also called short-circuiting;
		if (op.m_Type == TokenType::AND)
		{
			if (!isTruthy(leftOperand)) return leftOperand;
		}
		else if (op.m_Type == TokenType::OR)
		{
			if (isTruthy(rightOperand)) return rightOperand;
		}

		switch (op.m_Type)
		{
			//arithmetic operations;
			case TokenType::PLUS:
			case TokenType::PLUS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) + std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) + std::get<double>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) + std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::MINUS:
			case TokenType::MINUS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) - std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) - std::get<double>(rightOperand);

				throw error(op, "Operation must be done on a number.");
			}

			case TokenType::STAR:
			case TokenType::STAR_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) * std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) * std::get<double>(rightOperand);

				throw error(op, "Operation must be done on a number.");
			}

			case TokenType::SLASH:
			case TokenType::SLASH_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) / std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) / std::get<double>(rightOperand);

				throw error(op, "Operation must be done on a number.");
			}

			//logical and comparison operations;
			case TokenType::GREATER:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) > std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) > std::get<double>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) > std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) < std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) < std::get<double>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) < std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::GREATER_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) >= std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) >= std::get<double>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) >= std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) <= std::get<float>(rightOperand);
				else if (std::holds_alternative<double>(leftOperand) && std::holds_alternative<double>(rightOperand))
					return std::get<double>(leftOperand) <= std::get<double>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) <= std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::EQUAL_EQUAL: return leftOperand == rightOperand;
			case TokenType::BANG_EQUAL: return leftOperand != rightOperand;

			//logical operation
			case TokenType::AND: return rightOperand;
			case TokenType::OR: return rightOperand;

			default:
				throw error(op, "Unexpected Error Occured.");
		}

		return Nil{};
	}

	void Interpreter::DefineNativeFunctions()
	{
		//registering nativ functions
		//native clock func
		m_GlobalEnv->define("clock", CreateRef<ClockFunction>());

		//native print func
		m_GlobalEnv->define("println", CreateRef<PrintFunction>());
		//nativ pow func
		m_GlobalEnv->define("pow", CreateRef<PowFunc>());
		//nativ max func
		m_GlobalEnv->define("max", CreateRef<MaxFunc>());
		//nativ sqrt func
		m_GlobalEnv->define("sqrt", CreateRef<SqrtFunc>());
		//nativ min func
		m_GlobalEnv->define("min", CreateRef<MinFunc>());
		//native input func
		m_GlobalEnv->define("input", CreateRef<InputFunc>());
		//native type cast functions
		m_GlobalEnv->define("str", CreateRef<StrCastFunc>());
		m_GlobalEnv->define("float", CreateRef<FloatCastFunc>());
		m_GlobalEnv->define("double", CreateRef<DoubleCastFunc>());
		m_GlobalEnv->define("bool", CreateRef<BoolCastFunc>());

		//native type check func
		m_GlobalEnv->define("typeof", CreateRef<TypeCheckFunc>());
	}

	double Interpreter::validateIndex(const Token& token, const Literal& indexLiteral)
	{
		double index{};

		if (auto indexPtr{ std::get_if<float>(&indexLiteral) })
			index = static_cast<double>(*indexPtr);
		else if (auto indexPtr{ std::get_if<double>(&indexLiteral) })
			index = *indexPtr;
		else
			throw error(token, "Index must be a Number.");

		if (!std::isfinite(index))
			throw error(token, "Index must be finite.");

		if(std::floor(index) != index)
			throw error(token, "Index must be an Integer.");

		if (index < 0.0f)
			throw error(token, "Index cannot be negative.");

		return index;
	}

	//runtime error handling
	RuntimeError Interpreter::error(const Token& tk, const std::string& msg)
	{
		Diagnostic diag{
			DiagnosticSeverity::ERROR,
			DiagnosticPhase::RUNTIME,
			msg,
			{ m_SourceName, tk.m_Line, tk.m_Column, 0 }
		};

		m_DiagnosticEngine.Report(std::move(diag));

		return RuntimeError{};
	}

	NativeRuntimeError Interpreter::nativeError(const std::string& msg)
	{
		Diagnostic diag{
			DiagnosticSeverity::ERROR,
			DiagnosticPhase::RUNTIME,
			msg,
			{ }
		};

		return NativeRuntimeError{ msg };
	}
}