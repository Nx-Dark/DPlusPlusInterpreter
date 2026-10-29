#include "dpch.h"
#include "Tokenization.h"

#include "Interpreter/D++.h"

namespace DXX {

	Tokens Tokenizer::Tokenize(const std::string& sourceCode)
	{
		sourceStream.str(sourceCode);
		sourceStream.clear();
		sourceStream.seekg(0);

		line = 1;
		Tokens tokens{};

		char ch;

		while (sourceStream.get(ch)) {
			TokenType type{};
			std::string lexeme{ ch };
			Literal literal;

			if (ch == ' ' || ch == '\t' || ch == '\r') continue;
			else if (ch == '\n') { line++; continue; }

			if (punctuationChars.find(ch) != std::string::npos)
			{
				switch (ch)
				{
					case ';': type = TokenType::SEMICOLON; break;
					case '[': type = TokenType::LEFT_SQ_BRACE; break;
					case ']': type = TokenType::RIGHT_SQ_BRACE; break;
					case '}': type = TokenType::RIGHT_BRACE; break;
					case '{': type = TokenType::LEFT_BRACE; break;
					case ')': type = TokenType::RIGHT_PAREN; break;
					case '(': type = TokenType::LEFT_PAREN; break;
					case '.': type = TokenType::DOT; break;
					case ',': type = TokenType::COMMA; break;
					default: DXX::error(line, "Unknown Puncuation Operator!"); continue;
				}

			}
			else if (operatorChars.find(ch) != std::string::npos)
			{
				char next{ static_cast<char>(sourceStream.peek()) };

				switch (ch)
				{
					case '!': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::BANG_EQUAL; } else {type = TokenType::BANG;} break;
					case '=': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::EQUAL_EQUAL; } else {type = TokenType::EQUAL;} break;
					case '>': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::GREATER_EQUAL; } else {type = TokenType::GREATER;} break;
					case '<': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::LESS_EQUAL; } else {type = TokenType::LESS;} break;
					case '+': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::PLUS_EQUAL; } else {type = TokenType::PLUS;} break;
					case '-': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::MINUS_EQUAL; } else {type = TokenType::MINUS;} break;
					case '/':
						if (next == '=') 
						{ 
							lexeme += next; sourceStream.get();
							type = TokenType::SLASH_EQUAL; 
						} 
						else if (next == '/') {
							while (sourceStream.peek() != '\n' && sourceStream.peek() != EOF) sourceStream.get();
							continue;
						}
						else type = TokenType::SLASH; 

						break;

					case '*': if (next == '=') { lexeme += next; sourceStream.get(); type = TokenType::STAR_EQUAL; } else { type = TokenType::STAR; } break;
					default: DXX::error(line, "Unknown Operator/Character!"); continue;
				}

			}

			else {
				switch (ch)
				{
					case '"': {
						if(!string(lexeme)) continue;
						type = TokenType::STRING;
						break;
					}
					default:
						if (std::isdigit(ch)) {
							number(lexeme);
							type = TokenType::NUMBER;
						}
						else if (ch == '_' || std::isalpha(ch)) {
							identifier(lexeme);
							type = TokenType::IDENTIFIER;
						}

						else {
							DXX::error(line, "Unexpected Character!");
							continue;
						}

						break;
				}
			}

			//token handling;
			literal = Nil{};

			if (type == TokenType::NUMBER) {
				literal = std::stof(lexeme);
			}
			else if (type == TokenType::STRING) {
				literal = lexeme.substr(1, lexeme.length() - 2);
			}
			else if (type == TokenType::IDENTIFIER) {
				literal = Nil{};

				if (s_keywords.contains(lexeme)) {
					type = s_keywords.at(lexeme);

					if (type == TokenType::TRUE)
						literal = true;
					else if (type == TokenType::FALSE)
						literal = false;
				}
			}
			else if (type == TokenType::NONE)
				continue;

			tokens << Token{ type, lexeme, literal, line };

		}

		tokens << Token{ TokenType::END_OF_FILE, "", Nil{}, line };

		return tokens;
	}

	bool Tokenizer::string(std::string& lex)
	{
		while (sourceStream.peek() != '"' && sourceStream.peek() != EOF)
		{
			if (sourceStream.peek() == '\n') line++;
			lex += static_cast<char>(sourceStream.get());
		}

		if (sourceStream.peek() == EOF)
		{
			DXX::error(line, "Unterminated String!");
			return false;
		}

		//closing '"';
		lex += static_cast<char>(sourceStream.get());
		return true;
	}

	void Tokenizer::number(std::string& lex)
	{
		while (std::isdigit(sourceStream.peek())) {
			lex += sourceStream.get();
		}

		if (sourceStream.peek() == '.') {
			sourceStream.get();
			int next{ sourceStream.peek() };
			sourceStream.unget();

			if (std::isdigit(next)) {
				lex += static_cast<char>(sourceStream.get());

				while (std::isdigit(sourceStream.peek())) {
					lex += static_cast<char>(sourceStream.get());
				}
			}
		}
	}

	void Tokenizer::identifier(std::string& lex)
	{
		while (isalphNumeric(sourceStream.peek()) || sourceStream.peek() == '_') lex += sourceStream.get();
	}

}