#include "dpch.h"
#include "Resolver.h"

#include "AST/Statement.h"
#include "AST/Expression.h"

#include "Interpreter/Interpreter.h"

namespace DXX
{

	Resolver::Resolver(DiagnosticEngine& diagnosticEngine, Interpreter& interpreter)
		: m_DiagnosticEngine{ diagnosticEngine }, m_Interpreter{ interpreter }
	{

	}

	Resolver::~Resolver()
	{

	}

	void Resolver::Resolve(const std::vector<Ref<Stmt>>& statements)
	{

		try
		{
			for (const auto& stmt : statements)
			{
				Resolve(stmt);
			}
		}
		catch (const ResolveError&)
		{
			return;
		}
	}

	void Resolver::Resolve(const Expr& expr)
	{
		expr.Accept(*this);
	}

	void Resolver::Resolve(const Ref<Stmt>& stmt)
	{
		stmt->Accept(*this);
	}

	void Resolver::ResolveLocal(const Expr& expr, Token name)
	{
		for (int i{ (int)m_Scopes.size() - 1 }; i >= 0; i--)
		{
			if (m_Scopes._Get_container().at(i).contains(name.m_Lexeme))
			{
				m_Interpreter.Resolve(expr, m_Scopes.size() - 1 - i);
				return;
			}
		}
	}

	void Resolver::ResolveFunction(FunctionStmt& functionStmt, FunctionType funcType)
	{
		FunctionType enclosingFunction{ m_CurrentFunction };
		m_CurrentFunction = funcType;	

		BeginScope();
		for (const auto& tkn : functionStmt.m_Parameters)
		{
			Declare(tkn);
			Define(tkn);
		}

		Resolve(functionStmt.m_Body);
		EndScope();
		m_CurrentFunction = enclosingFunction;
	}

	//resolving expressions;
	void Resolver::visitUnary(const UnaryExpr& unaryExpr)
	{
		Resolve(*unaryExpr.m_Expr);
	}

	void Resolver::visitBinary(const BinaryExpr& binaryExpr)
	{
		Resolve(*binaryExpr.m_LeftExpr);
		Resolve(*binaryExpr.m_RightExpr);
	}

	void Resolver::visitVariable(const VariableExpr& varExpr)
	{
		if (!m_Scopes.empty())
		{
			auto& scope{ m_Scopes.top() };

			auto it{ scope.find(varExpr.m_Token.m_Lexeme) };

			if (it != scope.end() && !it->second)
				throw error(varExpr.m_Token, "Can't read local variable in its own initializer.");

		}

		ResolveLocal(varExpr, varExpr.m_Token);
	}

	void Resolver::visitAssignment(const AssignExpr& assignExpr)
	{
		Resolve(*assignExpr.m_Expr);
		ResolveLocal(assignExpr, assignExpr.m_Identifier);
	}

	void Resolver::visitCall(const CallExpr& callExpr)
	{
		Resolve(*callExpr.m_Callee);

		for (const auto& arg : callExpr.m_Arguments)
		{
			Resolve(*arg);
		}
	}

	void Resolver::visitGet(const GetExpr& getExpr)
	{
		Resolve(*getExpr.m_Object);
	}

	void Resolver::visitSet(const SetExpr& setExpr)
	{
		Resolve(*setExpr.m_Object);
		Resolve(*setExpr.m_Value);
	}

	void Resolver::visitThis(const ThisExpr& thisExpr)
	{
		if (m_CurrentClass == ClassType::NONE)
			throw error(thisExpr.m_Keyword, "Unexpected 'this' outside of class methods.");

		ResolveLocal(thisExpr, thisExpr.m_Keyword);
	}

	void Resolver::visitSuper(const SuperExpr& superExpr)
	{
		if (m_CurrentClass == ClassType::NONE)
			throw error(superExpr.m_Keyword, "Cannot use 'super' outside of class.");
		else if (m_CurrentClass != ClassType::SUBCLASS)
			throw error(superExpr.m_Keyword, "Cannot use 'super' in a class with no superclass.");

		ResolveLocal(superExpr, superExpr.m_Keyword);
	}

	void Resolver::visitList(const ListExpr& listExpr)
	{
		for (const auto& expr : listExpr.m_Elements)
		{
			if (expr)
				Resolve(*expr);
		}
	}

	void Resolver::visitIndex(const IndexExpr& indexExpr)
	{
		Resolve(*indexExpr.m_Object);
		Resolve(*indexExpr.m_Index);
	}

	void Resolver::visitIndexSet(const IndexSetExpr& indexSetExpr)
	{
		Resolve(*indexSetExpr.m_Object);
		Resolve(*indexSetExpr.m_Index);
		Resolve(*indexSetExpr.m_Value);
	}

	void Resolver::visitGrouping(const GroupingExpr& groupingExpr)
	{
		Resolve(*groupingExpr.m_Expr);
	}

	//resolving statements;
	void Resolver::visitPrintStmt(PrintStmt& printStmt)
	{
		Resolve(*printStmt.m_Expr);
	}

	void Resolver::visitExpressionStmt(ExpressionStmt& expressionStmt)
	{
		expressionStmt.m_Expr->Accept(*this);
	}

	void Resolver::visitVarStmt(VarStmt& varStmt)
	{
		Declare(varStmt.m_Token);
		if (varStmt.m_InitializerExpr) Resolve(*varStmt.m_InitializerExpr);
		Define(varStmt.m_Token);
	}

	void Resolver::visitIfStmt(IfStmt& ifStmt)
	{
		Resolve(*ifStmt.m_Condition);
		Resolve(ifStmt.m_ThenBranch);
		if (ifStmt.m_ElseBranch) Resolve(ifStmt.m_ElseBranch);
	}

	void Resolver::visitBlockStmt(BlockStmt& blockStmt)
	{
		BeginScope();
		Resolve(blockStmt.m_Statements);
		EndScope();
	}

	void Resolver::visitWhileStmt(WhileStmt& whileStmt)
	{
		BeginScope();
		Resolve(*whileStmt.m_Condition);
		Resolve(whileStmt.m_ThenBranch);
		EndScope();
	}

	void Resolver::visitForStmt(ForStmt& forStmt)
	{
		BeginScope();

		if (forStmt.m_Initializer) Resolve(forStmt.m_Initializer);
		if (forStmt.m_Condition) Resolve(*forStmt.m_Condition);
		if (forStmt.m_Increment) Resolve(*forStmt.m_Increment);

		Resolve(forStmt.m_Body);

		EndScope();
	}

	void Resolver::visitFunctionStmt(FunctionStmt& functionStmt)
	{
		Declare(functionStmt.m_Name);
		Define(functionStmt.m_Name);

		ResolveFunction(functionStmt, m_CurrentFunction);
	}
	
	void Resolver::visitClassStmt(ClassStmt& classStmt)
	{
		ClassType enclosingClass{ m_CurrentClass };
		m_CurrentClass = ClassType::CLASS;

		Declare(classStmt.m_Name);
		Define(classStmt.m_Name);

		if (classStmt.m_SuperClass && classStmt.m_Name.m_Lexeme == classStmt.m_SuperClass->m_Token.m_Lexeme)
			throw error(classStmt.m_SuperClass->m_Token, "A Class cannot inherit from itself.");

		if (classStmt.m_SuperClass)
		{
			m_CurrentClass = ClassType::SUBCLASS;
			Resolve(*classStmt.m_SuperClass);

			BeginScope();
			m_Scopes.top()["super"] = true;
		}

		BeginScope();
		m_Scopes.top()["this"] = true;

		for (const auto& method : classStmt.m_Methods)
		{
			FunctionType decl{ FunctionType::METHOD };
			Ref<FunctionStmt> funcStmt{ std::dynamic_pointer_cast<FunctionStmt>(method) };
			if (funcStmt)
			{
				if (funcStmt->m_Name.m_Lexeme == "construct")
					decl = FunctionType::INITIALIZER;
				ResolveFunction(*funcStmt, decl);
			}
		}

		EndScope();
		
		if (classStmt.m_SuperClass)
			EndScope();

		m_CurrentClass = enclosingClass;
	}

	void Resolver::visitReturnStmt(ReturnStmt& returnStmt)
	{
		if (returnStmt.m_Expr)
		{
			if (m_CurrentFunction == FunctionType::INITIALIZER)
				throw error(returnStmt.m_Keyword, "return not allowed inside an initializer.");

			Resolve(*returnStmt.m_Expr);
		}
	}

	//helper stuff
	void Resolver::BeginScope()
	{
		m_Scopes.push(std::unordered_map<std::string, bool>{});
	}

	void Resolver::EndScope()
	{
		m_Scopes.pop();
	}

	void Resolver::Declare(const Token& name)
	{
		if (m_Scopes.empty()) return;

		auto& scope{ m_Scopes.top() };

		if (scope.contains(name.m_Lexeme))
			throw error(name, "A Variable with the same name exists in this scope");

		m_Scopes.top()[name.m_Lexeme] = false;
	}

	void Resolver::Define(const Token& name)
	{
		if (m_Scopes.empty()) return;

		m_Scopes.top()[name.m_Lexeme] = true;
	}

	//erroir handling
	ResolveError Resolver::error(Token token, const std::string& msg)
	{
		Diagnostic diag{
			DiagnosticSeverity::ERROR,
			DiagnosticPhase::RESOLVING,
			msg,
			{ m_SourceName, token.m_Line, token.m_Column, 0 }
		};

		m_DiagnosticEngine.Report(std::move(diag));

		return ResolveError{};
	}
}