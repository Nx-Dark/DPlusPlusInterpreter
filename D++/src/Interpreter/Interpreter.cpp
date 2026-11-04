#include "dpch.h"
#include "Interpreter.h"

#include "AST/Expression.h"
#include "AST/Statement.h"

#include "Functions/FunctionObject.h"
#include "Functions/NativeFunctions/NativeFunctions.h"

namespace DXX
{

	static bool isTruthy(const Literal& literal)
	{
		if (std::holds_alternative<Nil>(literal)) return false;
		else if (std::holds_alternative<bool>(literal)) return std::get<bool>(literal);
		else if (std::holds_alternative<float>(literal) && std::get<float>(literal) == 0.0f) return false;
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

	void Interpreter::Interpret(std::vector<Scope<Stmt>> stmts)
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

		//for evalutaing 'and' and 'or' logical operators; also called short-circuiting;
		if (op.m_Type == TokenType::AND)
		{
			if (!isTruthy(leftOperand)) return leftOperand;
		}
		else if (op.m_Type == TokenType::OR)
		{
			if (isTruthy(leftOperand)) return leftOperand;
		}

		//evaluate rhs after
		Literal rightOperand{ binaryExpr.m_RightExpr->Accept(*this) };

		switch (op.m_Type)
		{
			//arithmetic operations;
			case TokenType::PLUS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) + std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) + std::get<std::string>(rightOperand);
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::MINUS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) - std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}
			case TokenType::STAR:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) * std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}
			case TokenType::SLASH:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) / std::get<float>(rightOperand);
				
				throw error(op, "Operation must be done on a number.");
			}

			//logical and comparison operations;
			case TokenType::GREATER:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) > std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) > std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) < std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) < std::get<std::string>(rightOperand);

				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::GREATER_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) >= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) >= std::get<std::string>(rightOperand);
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::LESS_EQUAL:
			{
				if (std::holds_alternative<float>(leftOperand) && std::holds_alternative<float>(rightOperand))
					return std::get<float>(leftOperand) <= std::get<float>(rightOperand);
				else if (std::holds_alternative<std::string>(leftOperand) && std::holds_alternative<std::string>(rightOperand))
					return std::get<std::string>(leftOperand) <= std::get<std::string>(rightOperand);
				
				throw error(op, "Operation must be done on two number or two string.");
			}
			case TokenType::EQUAL_EQUAL: return leftOperand == rightOperand;
			case TokenType::BANG_EQUAL : return leftOperand != rightOperand;
			
			//logical operation
			case TokenType::AND: return rightOperand;
			case TokenType::OR : return rightOperand;

			default:
				throw error(op, "Unexpected Error Occured.");
		}

		return Nil{};
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

		//if (m_Environment->assign(assignExpr.m_Identifier.m_Lexeme, value))
		//	return value;
		//
		//throw error(assignExpr.m_Identifier, "Undefined Variable.");

		auto it{ m_Locals.find(&assignExpr) };

		if (it != m_Locals.end())
		{
			int distance{ it->second };
			m_Environment->assignAt(distance, assignExpr.m_Identifier.m_Lexeme, value);
		}

		else
		{
			m_GlobalEnv->assign(assignExpr.m_Identifier.m_Lexeme, value);
		}

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

		auto functionObj{ std::get_if<Ref<Callable>>(&callee) };

		if (functionObj)
		{
			size_t funcObjArity{ (*functionObj)->arity() };
			size_t calleeArgCount{ callExpr.m_Arguments.size() };

			if (funcObjArity != VARIADIC_ARITY && funcObjArity != calleeArgCount)
			{
				throw error(callExpr.m_ClosingParen, "Expected " + std::to_string(funcObjArity) + " Arguments, but got " + std::to_string(calleeArgCount) + " Instead");
			}

			try 
			{
				return (*functionObj)->Call(*this, arguments);
			}

			catch (const NativeRuntimeError& nre)
			{
				throw error(callExpr.m_ClosingParen, nre.msg);
			}
		}

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

		auto refFuncStatement{ CreateRef<FunctionStmt>(std::move(functionStmt.m_Name), std::move(functionStmt.m_Body), std::move(functionStmt.m_Parameters)) };

		m_Environment->define(fName.m_Lexeme, 
			CreateRef<FunctionObject>(refFuncStatement, m_Environment)
		);
	}

	Literal Interpreter::lookUpVariable(Token token, const Expr& expr)
	{
		auto it{ m_Locals.find(&expr) };

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

		m_DiagnosticEngine.Report(std::move(diag));

		return NativeRuntimeError{};
	}
}