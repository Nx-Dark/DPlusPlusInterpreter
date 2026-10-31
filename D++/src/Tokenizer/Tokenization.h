#pragma once

#include "Literal.h"
#include "Token.h"

#include "Diagnostics/DiagnosticEngine.h"

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
		inline static std::size_t line{ 1 };
		inline static std::size_t column{ 1 };

	public:
		static Tokens Tokenize(const std::string& sourceCode, const std::string& sourceName, DiagnosticEngine& diagnosticEngine);

	private:
		static bool string(std::string& lex, const std::string& sourceName, DiagnosticEngine& diagnosticEngine, std::size_t startLine, std::size_t startColumn);
		static bool number(std::string& lex, const std::string& sourceName, DiagnosticEngine& diagnosticEngine, std::size_t startLine, std::size_t startColumn);
		static bool identifier(std::string& lex);

		static char consume();

		inline static bool isalphNumeric(int ch) { 
			return (ch != EOF) && (std::isalnum(static_cast<unsigned char>(ch))); 
		}
	};

}