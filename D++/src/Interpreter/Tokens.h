#pragma once

namespace DXX {

	enum class TokeType {

		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
		COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

		// One or two character tokens.
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL,

		// Literals.
		IDENTIFIER, STRING, NUMBER,

		// Keywords.
		AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
		PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	};

	class Tokens {
		std::vector<std::string> m_Tokens;

		inline static const std::string punctuationChars{";[]{}().,"};
		inline static const std::string operatorChars{"+-*/<>=!"};

	public:
		Tokens() = default;
		~Tokens() = default;

		inline const std::vector<std::string>& GetTokens() const { return m_Tokens; }
		inline std::string at(int _index) const { return m_Tokens.at(_index); }

		inline std::string operator[](int index) const { return m_Tokens[index]; }
		inline void operator<<(const std::string& token) { m_Tokens.push_back(token); }

		inline void push_token(const std::string& token) { m_Tokens.push_back(token); }
		inline void remove_token(const std::string& token) {
			auto it{ std::find(m_Tokens.begin(), m_Tokens.end(), token) };
			if (it != m_Tokens.end()) {
				m_Tokens.erase(it);
			}
		}

		inline std::vector<std::string>::iterator begin() { return m_Tokens.begin(); }
		inline std::vector<std::string>::iterator end() { return m_Tokens.end(); }
		inline std::vector<std::string>::const_iterator cbegin() const { return m_Tokens.cbegin(); }
		inline std::vector<std::string>::const_iterator cend() const { return m_Tokens.cend(); }

		static Tokens Tokenizer(const std::string& sourceCode);
	};

}