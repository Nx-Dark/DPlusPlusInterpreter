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

		DiagnosticEngine& m_DiagnosticEngine;
		std::string m_SourceName{};

	public:
		Parser() = default;
		Parser(Tokens tokens, DiagnosticEngine& m_DiagnosticEngine, const std::string& srcName);
		Parser(const Parser&) = delete;
		~Parser() = default;

		std::vector<Scope<Stmt>> Parse();

	private:
		//defining grammar rules for the parser;
		//pasring expression
		Scope<Expr> expression();
		Scope<Expr> logicalOr();
		Scope<Expr> logicalAnd();
		Scope<Expr> equality();
		Scope<Expr> comparison();
		Scope<Expr> term();
		Scope<Expr> factor();
		Scope<Expr> unary();
		Scope<Expr> primary();
		
		//parsing statements;
		Scope<Stmt> statement();
		Scope<Stmt> printStatement();
		Scope<Stmt> expressionStatement();

		//helper functions;
		//to easily navigate in the token collection;
		const Token& peek() const;
		const Token& previous() const;

		Token advance();
		Token consume(TokenType type, const std::string& msg);
		bool check(TokenType type) const;
		bool match(TokenType type);
		bool isAtEnd() const;

		//error handling
		ParseError error(const Token& token, const std::string& message);
	};

}