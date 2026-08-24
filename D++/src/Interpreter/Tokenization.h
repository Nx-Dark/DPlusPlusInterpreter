#pragma once

namespace DXX {

	inline std::string TokenTypeToString(TokenType type) {
		switch (type) {
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

	inline std::string LiteralToString(const Literal& literal)
	{

	}

	enum class TokenType {

		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
		LEFT_SQ_BRACE, RIGHT_SQ_BRACE,
		COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

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

	struct Nil {
		bool operator==(const Nil&) const = default;
	};
	using Literal = std::variant<Nil, float, std::string, bool>;

	struct Token {
		TokenType m_Type{};
		std::string m_Lexeme{};
		Literal m_Literal;
		int m_Line{};

		bool operator==(const Token&) const = default;

		Token(TokenType type, std::string& lexeme, Literal literal, int line)
			: m_Type(type), m_Lexeme(std::move(lexeme)), m_Literal(literal), m_Line(line) {

		}
	};

	class Tokens {
		std::vector<Token> m_Tokens;

	public:
		Tokens() = default;
		~Tokens() = default;

		inline const std::vector<Token>& GetTokens() const { return m_Tokens; }
		inline Token& at(int _index) { return m_Tokens.at(_index); }

		inline Token operator[](int index) const { return m_Tokens[index]; }
		inline void operator<<(const Token& token) { m_Tokens.push_back(token); }

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
		
		//start and current tokens
		int line{ 1 }; //which line the token is located at

		std::stringstream m_SC{};
	public:
		Tokenizer(const std::string& sourceCode);

		void Tokenize(Tokens& tokens);

	private:
		void string(std::string& lex);
		void number(std::string& lex);
		void identifier(std::string& lex);

		inline bool isalphNumeric(char ch) { return std::isalpha(ch) || std::isdigit(ch); }
	};

}