#include "dpch.h"
#include "Tokenization.h"

namespace DXX {

	static void DiagnosticReportErrors(DiagnosticEngine& engine, DiagnosticSeverity severity, const std::string& message, SourceLocation srcLocation)
	{
		Diagnostic errorReport{ severity, DiagnosticPhase::LEXING, message, srcLocation };
		engine.Report(std::move(errorReport));
	}

	Tokens Tokenizer::Tokenize(const std::string& sourceCode, const std::string& sourceName, DiagnosticEngine& diagnosticEngine)
	{
		sourceStream.str(sourceCode);
		sourceStream.clear();
		sourceStream.seekg(0);

		line = 1;
		column = 1;
		Tokens tokens{};

		while (sourceStream.peek() != EOF) {

			const std::size_t tokenLine{ line };
			const std::size_t tokenColumn{ column };

			char ch{ consume() };

			TokenType type{};
			std::string lexeme{ ch };
			Literal literal;

			if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') { continue; }

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
					default: continue;
				}

			}
			else if (operatorChars.find(ch) != std::string::npos)
			{
				char next{ static_cast<char>(sourceStream.peek()) };

				switch (ch)
				{
					case '!': if (next == '=') { lexeme += consume(); type = TokenType::BANG_EQUAL; } else {type = TokenType::BANG;} break;
					case '=': if (next == '=') { lexeme += consume(); type = TokenType::EQUAL_EQUAL; } else {type = TokenType::EQUAL;} break;
					case '>': if (next == '=') { lexeme += consume(); type = TokenType::GREATER_EQUAL; } else {type = TokenType::GREATER;} break;
					case '<': if (next == '=') { lexeme += consume(); type = TokenType::LESS_EQUAL; } else {type = TokenType::LESS;} break;
					case '+': if (next == '=') { lexeme += consume(); type = TokenType::PLUS_EQUAL; } else {type = TokenType::PLUS;} break;
					case '-': if (next == '=') { lexeme += consume(); type = TokenType::MINUS_EQUAL; } else {type = TokenType::MINUS;} break;
					case '/':
						if (next == '=') 
						{ 
							lexeme += consume();
							type = TokenType::SLASH_EQUAL; 
						} 
						else if (next == '/') {
							while (sourceStream.peek() != '\n' && sourceStream.peek() != EOF) consume();
							continue;
						}
						else type = TokenType::SLASH; 

						break;

					case '*': if (next == '=') { lexeme += consume(); type = TokenType::STAR_EQUAL; } else { type = TokenType::STAR; } break;
					default: continue;
				}

			}

			else {
				switch (ch)
				{
					case '"': {
						if(!string(lexeme, sourceName, diagnosticEngine, tokenLine, tokenColumn)) continue;
						type = TokenType::STRING;
						break;
					}
					default:
						if (std::isdigit(static_cast<unsigned char>(ch))) {
							if (!number(lexeme, sourceName, diagnosticEngine, tokenLine, tokenColumn)) continue;
							type = TokenType::NUMBER;
						}
						else if (ch == '_' || std::isalpha(static_cast<unsigned char>(ch))) {
							identifier(lexeme);
							type = TokenType::IDENTIFIER;
						}

						else {
							DiagnosticReportErrors(diagnosticEngine, DiagnosticSeverity::ERROR, std::string{ "Unexpected Character: '" } + ch + "'",
								{ sourceName, tokenLine, tokenColumn, 0 });

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

			tokens << Token{ type, lexeme, literal, tokenLine, tokenColumn };

		}

		tokens << Token{ TokenType::END_OF_FILE, "", Nil{}, line, column };

		return tokens;
	}

	char Tokenizer::consume()
	{
		char ch{ static_cast<char>(sourceStream.get()) };

		if (ch == '\n')
		{
			line++;
			column = 1;
		}
		else
			column++;

		return ch;
	}

	bool Tokenizer::string(std::string& lex, const std::string& sourceName, DiagnosticEngine& diagnosticEngine, std::size_t startLine, std::size_t startColumn)
	{
		while (sourceStream.peek() != '"' && sourceStream.peek() != EOF)
		{
			lex += consume();
		}

		if (sourceStream.peek() == EOF)
		{
			DiagnosticReportErrors(diagnosticEngine, DiagnosticSeverity::ERROR, "Unterminated String Literal.",
				{ sourceName, startLine, startColumn, 0 });

			return false;
		}

		//closing '"';
		lex += consume();
		return true;
	}

	bool Tokenizer::number(std::string& lex, const std::string& sourceName, DiagnosticEngine& diagnosticEngine, std::size_t startLine, std::size_t startColumn)
	{
		while (std::isdigit(static_cast<unsigned char>(sourceStream.peek()))) {
			lex += consume();
		}

		if (sourceStream.peek() == '.') {
			 
			sourceStream.get();
			int next{ sourceStream.peek() };
			sourceStream.unget();

			if (std::isdigit(static_cast<unsigned char>(next))) {
				lex += consume();

				while (std::isdigit(static_cast<unsigned char>(sourceStream.peek()))) {
					lex += consume();
				}
				
				if (sourceStream.peek() == '.')
				{
					sourceStream.get();
					next = sourceStream.peek();
					sourceStream.unget();

					if (std::isdigit(static_cast<unsigned char>(next)) || next == '.')
					{
						while (std::isdigit(static_cast<unsigned char>(sourceStream.peek())) || sourceStream.peek() == '.')
							consume();

						DiagnosticReportErrors(diagnosticEngine, DiagnosticSeverity::ERROR, "Invalid numeric literal.",
							{ sourceName, startLine, startColumn, 0 });

						return false;
					}
				}

			}
			
		}

		//identifier issues with numbers;
		if (std::isalpha(static_cast<unsigned char>(sourceStream.peek())) || sourceStream.peek() == '_')
		{
			DiagnosticReportErrors(diagnosticEngine, DiagnosticSeverity::ERROR, "Invalid numeric literal.",
				{ sourceName, startLine, startColumn, 0 });

			while (isalphNumeric(sourceStream.peek()) || sourceStream.peek() == '_' || sourceStream.peek() == '.')
				consume();

			return false;
		}

		return true;
	}

	bool Tokenizer::identifier(std::string& lex)
	{
		while (isalphNumeric(sourceStream.peek()) || sourceStream.peek() == '_') lex += consume();

		return true;
	}

}