#pragma once

#include "Literal.h"
#include <cstddef>

namespace DXX
{

	//token type representation
	enum class TokenType {

		NONE = 0,

		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
		LEFT_SQ_BRACE, RIGHT_SQ_BRACE,
		COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
		//BITWISE_AND, BITWISE_OR, BITWISE_XOR, BITWISE_LEFT_SHIFT, BITWISE_RIGHT_SHIFT, //Future

		// One or two character tokens.
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL,
		PLUS_EQUAL, MINUS_EQUAL, SLASH_EQUAL, STAR_EQUAL,

		// Literals.
		IDENTIFIER, STRING, NUMBER,

		// Keywords.
		AND, CLASS, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
		PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

		END_OF_FILE

	};

	//to convert from tokentype to string 
	inline std::string TokenTypeToString(TokenType type) {
		switch (type) {
		case TokenType::NONE: return "NONE";
		case TokenType::LEFT_PAREN: return "LEFT_PAREN";
		case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
		case TokenType::LEFT_BRACE: return "LEFT_BRACE";
		case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
		case TokenType::LEFT_SQ_BRACE: return "LEFT_SQ_BRACE";
		case TokenType::RIGHT_SQ_BRACE: return "RIGHT_SQ_BRACE";
		case TokenType::COMMA: return "COMMA";
		case TokenType::DOT: return "DOT";
		case TokenType::MINUS: return "MINUS";
		case TokenType::PLUS: return "PLUS";
		case TokenType::SEMICOLON: return "SEMICOLON";
		case TokenType::SLASH: return "SLASH";
		case TokenType::STAR: return "STAR";
		//case TokenType::BITWISE_AND: return "BITWISE_AND";
		//case TokenType::BITWISE_OR: return "BITWISE_OR";
		//case TokenType::BITWISE_XOR: return "BITWISE_XOR";
		//case TokenType::BITWISE_LEFT_SHIFT: return "BITWISE_LEFT_SHIFT";
		//case TokenType::BITWISE_RIGHT_SHIFT: return "BITWISE_RIGHT_SHIFT";
		case TokenType::BANG: return "BANG";
		case TokenType::BANG_EQUAL: return "BANG_EQUAL";
		case TokenType::EQUAL: return "EQUAL";
		case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
		case TokenType::GREATER: return "GREATER";
		case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
		case TokenType::LESS: return "LESS";
		case TokenType::LESS_EQUAL: return "LESS_EQUAL";
		case TokenType::PLUS_EQUAL: return "PLUS_EQUAL";
		case TokenType::MINUS_EQUAL: return "MINUS_EQUAL";
		case TokenType::SLASH_EQUAL: return "SLASH_EQUAL";
		case TokenType::STAR_EQUAL: return "STAR_EQUAL";
		case TokenType::IDENTIFIER: return "IDENTIFIER";
		case TokenType::STRING: return "STRING";
		case TokenType::NUMBER: return "NUMBER";
		case TokenType::AND: return "AND";
		case TokenType::CLASS: return "CLASS";
		case TokenType::ELSE: return "ELSE";
		case TokenType::FALSE: return "FALSE";
		case TokenType::FUNC: return "FUNC";
		case TokenType::FOR: return "FOR";
		case TokenType::IF: return "IF";
		case TokenType::NIL: return "NIL";
		case TokenType::OR: return "OR";
		case TokenType::PRINT: return "PRINT";
		case TokenType::RETURN: return "RETURN";
		case TokenType::SUPER: return "SUPER";
		case TokenType::THIS: return "THIS";
		case TokenType::TRUE: return "TRUE";
		case TokenType::VAR: return "VAR";
		case TokenType::WHILE: return "WHILE";
		case TokenType::END_OF_FILE: return "END_OF_FILE";
		default: return "UNKNOWN";
		}
	}

	//token struct
	struct Token {
		TokenType m_Type{};
		std::string m_Lexeme{};
		Literal m_Literal{};
		std::size_t m_Line{};
		std::size_t m_Column{};

		Token(TokenType type, std::string lexeme, Literal literal, std::size_t line, std::size_t column)
			: m_Type(type), m_Lexeme(std::move(lexeme)), m_Literal(std::move(literal)), m_Line(line), m_Column{ column } {

		}

		bool operator==(const Token&) const = default;

		std::string toString() const {
			return "Line: " + std::to_string(m_Line) + ", Column: " + std::to_string(m_Column) + ", Lex: " + m_Lexeme + ", Literal: " + LiteralToString(m_Literal) + ", TokenType: " + TokenTypeToString(m_Type);
		}
	};

	//array of tokens
	class Tokens {
		std::vector<Token> m_Tokens;

	public:
		Tokens() = default;
		~Tokens() = default;

		inline const std::vector<Token>& GetTokens() const { return m_Tokens; }
		inline Token& at(size_t _index) { return m_Tokens.at(_index); }
		inline const Token& at(size_t _index) const { return m_Tokens.at(_index); }
		inline Token& operator[](size_t index) { return m_Tokens[index]; }
		inline const Token& operator[](size_t index) const { return m_Tokens[index]; }

		inline void operator<<(const Token& token) { m_Tokens.push_back(token); }
		inline void operator<<(Token&& token) { m_Tokens.push_back(std::move(token)); }

		inline void push_token(const Token& token) { m_Tokens.push_back(token); }
		inline void remove_token(const Token& token) {
			auto it{ std::find(m_Tokens.begin(), m_Tokens.end(), token) };
			if (it != m_Tokens.end()) {
				m_Tokens.erase(it);
			}
		}

		inline std::vector<Token>::iterator begin() { return m_Tokens.begin(); }
		inline std::vector<Token>::iterator end() { return m_Tokens.end(); }
		inline std::vector<Token>::const_iterator cbegin() const { return m_Tokens.cbegin(); }
		inline std::vector<Token>::const_iterator cend() const { return m_Tokens.cend(); }
	};

}