#pragma once

#include "Literal.h"
#include "Token.h"

namespace DXX {

	class Tokenizer
	{
		inline static const std::string punctuationChars{ ";[]{}().," };
		inline static const std::string operatorChars{ "+-*/<>=!" };

		inline static const std::unordered_map<std::string, TokenType> s_keywords{
			{"and", TokenType::AND},
			{"if", TokenType::IF},
			{"else", TokenType::ELSE},
			{"while", TokenType::WHILE},
			{"for", TokenType::FOR},
			{"NIL", TokenType::NIL},
			{"print", TokenType::PRINT},
			{"or", TokenType::OR},
			{"func", TokenType::FUNC},
			{"class", TokenType::CLASS},
			{"return", TokenType::RETURN},
			{"super", TokenType::SUPER},
			{"this", TokenType::THIS},
			{"var", TokenType::VAR},
			{"False", TokenType::FALSE},
			{"True", TokenType::TRUE}
		};

		inline static std::stringstream sourceStream{};
		inline static int line{ 1 };

	public:
		static Tokens Tokenize(const std::string& sourceCode);

	private:
		static bool string(std::string& lex);
		static void number(std::string& lex);
		static void identifier(std::string& lex);

		inline static bool isalphNumeric(int ch) { 
			return ch != EOF && (std::isalpha(static_cast<unsigned char>(ch)) || std::isdigit(static_cast<unsigned char>(ch))); 
		}
	};

}