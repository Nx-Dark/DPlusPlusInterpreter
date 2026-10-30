#include "dpch.h"
#include "Parser.h"

namespace DXX
{

	Parser::Parser(Tokens tokens)
		: m_Tokens{ std::move(tokens) } { }

	Scope<Expr> Parser::Parse()
	{
		return expression();
	}

	//defining functions for grammar rules for the parser
	Scope<Expr> Parser::expression()
	{
		return equality();
	}

	Scope<Expr> Parser::equality()
	{

		Scope<Expr> expr{ comparison() };

		while (check(TokenType::BANG_EQUAL) || check(TokenType::EQUAL_EQUAL))
		{

			Token op{ advance() };

			Scope<Expr> rightExpr{ comparison() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));

		}

		return expr;
	}

	Scope<Expr> Parser::comparison()
	{
		Scope<Expr> expr{ term() };

		while (check(TokenType::GREATER_EQUAL) || check(TokenType::LESS_EQUAL) || check(TokenType::GREATER) || check(TokenType::LESS))
		{

			Token op{ advance() };

			Scope<Expr> rightExpr{ term() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));
		}

		return expr;
	}

	Scope<Expr> Parser::term()
	{
		Scope<Expr> expr{ factor() };

		while (check(TokenType::PLUS) || check(TokenType::MINUS))
		{
			Token op{ advance() };

			Scope<Expr> rightExpr{ factor() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));
		}

		return expr;
	}

	Scope<Expr> Parser::factor()
	{
		Scope<Expr> expr{ unary() };

		while (check(TokenType::STAR) || check(TokenType::SLASH))
		{
			Token op{ advance() };

			Scope<Expr> rightExpr{ unary() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));
		}

		return expr;
	}

	Scope<Expr> Parser::unary()
	{
		Token op{ peek() };

		if (check(TokenType::BANG) || check(TokenType::MINUS))
		{
			advance();
			Scope<Expr> rightExpr{ unary() };
			return CreateScope<UnaryExpr>(std::move(op), std::move(rightExpr));
		}

		return primary();
	}

	Scope<Expr> Parser::primary()
	{
		//check for primary data and construct Expressions for it
		//such as LiteralExpr(NUMBER, STRING, NIL) and GroupedExpr, enclosing in '()', which might contain an expression in it;

		Token token{ advance() };

		switch (token.m_Type)
		{
			case TokenType::NUMBER:
			case TokenType::STRING:
			case TokenType::NIL   :
			case TokenType::TRUE  :
			case TokenType::FALSE : return CreateScope<LiteralExpr>(std::move(token.m_Literal));
			
			case TokenType::LEFT_PAREN:
			{
				Scope<Expr> expr{ expression() };
				if (!check(TokenType::RIGHT_PAREN)) {
					std::println("Error");
					break;
				}

				advance();
				return CreateScope<GroupingExpr>(std::move(expr));
			}
			default:
				//temporary;
				std::println("Eror");
				break;
		}

		return {};
	}

	//defining helper functions;
	const Token& Parser::peek() const
	{
		return m_Tokens[m_CurTknIdx];
	}
	const Token& Parser::previous() const
	{
		return (m_CurTknIdx == 0) ? m_Tokens[m_CurTknIdx] : m_Tokens[m_CurTknIdx - 1];
	}

	Token Parser::advance()
	{
		if (!isAtEnd())
			++m_CurTknIdx;

		return previous();
	}

	//checking the token type of the next boutta be consumed token
	bool Parser::check(TokenType type) const
	{
		if (isAtEnd()) return false;

		return peek().m_Type == type;
	}

	bool Parser::match(TokenType type)
	{
		if (check(type))
		{
			advance(); 
			return true;
		}

		return false;
		
	}

	bool Parser::isAtEnd() const
	{
		return peek().m_Type == TokenType::END_OF_FILE;
	}

}