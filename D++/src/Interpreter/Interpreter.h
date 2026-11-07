#pragma once

#include "AST/EvaluatorVisitor.h"
#include "AST/StatementVisitor.h"

#include "Diagnostics/Diagnostic.h"
#include "Diagnostics/DiagnosticEngine.h"

#include "Environment.h"

#include "AST/IdType.h"

namespace DXX
{

	struct Token;

	struct RuntimeError {};
	struct NativeRuntimeError { std::string msg; };

	class Interpreter : public EvaluatorVisitor, public StmtVisitor
	{

	private:

		DiagnosticEngine& m_DiagnosticEngine;
		std::string m_SourceName{};

		//environment;
		const Ref<Environment> m_GlobalEnv{};
		Ref<Environment> m_Environment{};

		//locals(expressions)
		std::unordered_map<ExprID, int> m_Locals{}; //int the depth, where a particular statement with this expression id is located

		friend class EnvironmentGuard;
		friend class FunctionObject;
		friend class Callable;

	public:
		Interpreter(DiagnosticEngine& engine);
		inline void SetSourceName(const std::string& sourceName) { m_SourceName = sourceName; }

		virtual ~Interpreter() = default;

		//entry point to the evalutaion
		void Interpret(const Expr& expr);
		void Interpret(std::vector<Ref<Stmt>> stmts);

		//expression visitor functions
		virtual Literal visitLiteral(const LiteralExpr& literalExpr) override;
		virtual Literal visitUnary(const UnaryExpr& unaryExpr) override;
		virtual Literal visitBinary(const BinaryExpr& binaryExpr) override;
		virtual Literal visitGrouping(const GroupingExpr& groupingExpr) override;
		virtual Literal visitVariable(const VariableExpr& variableExpr) override;
		virtual Literal visitAssignment(const AssignExpr& assignExpr) override;
		virtual Literal visitCall(const CallExpr& callExpr) override;
		virtual Literal visitGet(const GetExpr& getExpr) override;
		virtual Literal visitSet(const SetExpr& getExpr) override;
		virtual Literal visitThis(const ThisExpr& thisExpr) override;
		virtual Literal visitSuper(const SuperExpr& superExpr) override;

		//statement visitor functions;
		virtual void visitExpressionStmt(ExpressionStmt& expressionStmt) override;
		virtual void visitPrintStmt(PrintStmt& printStmt) override;
		virtual void visitVarStmt(VarStmt& varStmt) override;
		virtual void visitBlockStmt(BlockStmt& blockStmt) override;
		virtual void visitIfStmt(IfStmt& ifStmt) override;
		virtual void visitWhileStmt(WhileStmt& whileStmt) override;
		virtual void visitForStmt(ForStmt& forStmt) override;
		virtual void visitBreakStmt(BreakStmt&) override;
		virtual void visitContinueStmt(ContinueStmt&) override;
		virtual void visitReturnStmt(ReturnStmt& returnstmt) override;
		virtual void visitFunctionStmt(FunctionStmt& functionStmt) override;
		virtual void visitClassStmt(ClassStmt& classStmt) override;

		//resolve for resolver
		void Resolve(const Expr& expr, int depth);

		NativeRuntimeError nativeError(const std::string& msg);
	private:
		RuntimeError error(const Token& tk, const std::string& msg);

		Literal lookUpVariable(Token name, const Expr& expr);

		void DefineNativeFunctions();

		//break and continue internal signals;
		struct BreakSignal {};
		struct ContinueSignal {};
		struct ReturnSignal {
			Literal value{};
		};
	};

}