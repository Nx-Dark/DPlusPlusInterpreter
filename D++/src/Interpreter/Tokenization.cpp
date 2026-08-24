#include "dpch.h"
#include "Tokenization.h"

#include "D++.h"

namespace DXX {

	Tokenizer::Tokenizer(const std::string& sourceCode)
		: m_SC(sourceCode)
	{
	}


	void Tokenizer::Tokenize(Tokens& tokens)
	{
		char ch;

		//inline static const std::string punctuationChars{ ";[]{}().," };
		//inline static const std::string operatorChars{ "+-*/<>=!" };

		while (m_SC.get(ch)) {
			TokenType type{};
			std::string lexeme{ ch };
			Literal literal;

			if (ch == ' ' || ch == '\t' || ch == '\r') continue;
			if (ch == '\n') { line++; continue; }

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
					default: DXX::error(line, "Unknown Puncuation Operator!");

				}

			}
			else if (operatorChars.find(ch) != std::string::npos)
			{
				char next{ static_cast<char>(m_SC.peek()) };

				switch (ch)
				{
					case '!': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::BANG_EQUAL; } else {type = TokenType::BANG;} break;
					case '=': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::EQUAL_EQUAL; } else {type = TokenType::EQUAL;} break;
					case '>': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::GREATER_EQUAL; } else {type = TokenType::GREATER;} break;
					case '<': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::LESS_EQUAL; } else {type = TokenType::LESS;} break;
					case '+': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::PLUS_EQUAL; } else {type = TokenType::PLUS;} break;
					case '-': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::MINUS_EQUAL; } else {type = TokenType::MINUS;} break;
					case '/':
						if (next == '=') 
						{ 
							lexeme += next; m_SC.get();
							type = TokenType::SLASH_EQUAL; 
						} 
						else if (next == '/') {
							while (m_SC.peek() != '\n' && m_SC.peek() != EOF) m_SC.get();
							continue;
						}
						else type = TokenType::SLASH; 

						break;

					case '*': if (next == '=') { lexeme += next; m_SC.get(); type = TokenType::STAR_EQUAL; } else { type = TokenType::STAR; } break;
					default: DXX::error(line, "Unknown Operator/Character!");
				}

			}

			else {
				switch (ch)
				{
					case '"': string(lexeme); type = TokenType::STRING; break;
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
				if (s_keywords.find(lexeme) != s_keywords.end()) {
					type = s_keywords.at(lexeme);
				}
			}

			tokens << (Token(type, lexeme, literal, line));

		}
	}

	void Tokenizer::string(std::string& lex)
	{
		while (m_SC.peek() != '"' && m_SC.peek() != EOF)
		{
			if (m_SC.peek() == '\n') line++;
			lex += m_SC.get();
		}

		if (m_SC.peek() == EOF)
		{
			DXX::error(line, "Unterminated String!");
			return;
		}

		//closing '"';
		lex += m_SC.get();
	}

	void Tokenizer::number(std::string& lex)
	{
		while (std::isdigit(m_SC.peek())) {
			lex += m_SC.get();
		}

		if (m_SC.peek() == '.') {
			m_SC.get();
			char next{ static_cast<char>(m_SC.peek()) };
			m_SC.unget();

			if (std::isdigit(next)) {
				lex += m_SC.get();

				while (std::isdigit(m_SC.peek())) {
					lex += m_SC.get();
				}
			}
		}
	}

	void Tokenizer::identifier(std::string& lex)
	{
		while (isalphNumeric(m_SC.peek()) || m_SC.peek() == '_') lex += m_SC.get();

	}

}