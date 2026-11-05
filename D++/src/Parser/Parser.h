#pragma once

#include "AST/Expression.h"
#include "AST/Statement.h"

#include "Diagnostics/Diagnostic.h"
#include "Diagnostics/DiagnosticEngine.h"

namespace DXX
{

	class Parser
	{
	private:

		//parse error struct
		struct ParseError {};

		Tokens m_Tokens{};
		size_t m_CurTknIdx{}; //points to the next token aboutta be consumed in the token collection;
		size_t m_LoopDepth{}; //increments if the parser is inside a loop body;
		size_t m_FunctionDepth{};

		DiagnosticEngine& m_DiagnosticEngine;
		std::string m_SourceName{};

	public:
		Parser() = default;
		Parser(Tokens tokens, DiagnosticEngine& m_DiagnosticEngine, const std::string& srcName);
		Parser(const Parser&) = delete;
		~Parser() = default;

		std::vector<Ref<Stmt>> Parse();

	private:
		//defining grammar rules for the parser;
		//pasring expression
		Ref<Expr> expression();
		Ref<Expr> assignment();
		Ref<Expr> logicalOr();
		Ref<Expr> logicalAnd();
		Ref<Expr> equality();
		Ref<Expr> comparison();
		Ref<Expr> term();
		Ref<Expr> factor();
		Ref<Expr> unary();
		Ref<Expr> call();
		Ref<Expr> primary();

		Ref<Expr> finishCall(Ref<Expr> callee);
		
		//parsing statements;
		Ref<Stmt> statement();
		Ref<Stmt> declaration(); //for variable decl, class decl, func decl and stuff
		Ref<Stmt> expressionStatement();
		Ref<Stmt> ifStatement();
		Ref<Stmt> printStatement();
		Ref<Stmt> whileStatement();
		Ref<Stmt> forStatement();
		Ref<Stmt> breakStatement();
		Ref<Stmt> continueStatement();
		Ref<Stmt> returnStatement();
		Ref<Stmt> varDeclaration(bool consumeSemicolon = true); //var statement;
		Ref<Stmt> classDeclaraton();
		Ref<Stmt> functionDeclaration(const std::string& kind);
		std::vector<Ref<Stmt>> block();

		//helper functions;
		//to easily navigate in the token collection;
		const Token& peek() const;
		const Token& previous() const;

		Token advance();
		Token consume(TokenType type, const std::string& msg);
		bool check(TokenType type) const;
		template<typename... Types>
		inline bool match(Types... types)
		{
			if ((check(types) || ...))
			{
				advance();
				return true;
			}

			return false;
		}
		bool isAtEnd() const;

		//error handling
		ParseError error(const Token& token, const std::string& message);
		void synchronize();
	};

}