#include "dpch.h"
#include "Parser.h"

namespace DXX
{

	struct LoopDepthGuard
	{
	private:
		size_t& m_LoopDepth;

	public:
		explicit LoopDepthGuard(size_t& depth)
			: m_LoopDepth{ depth } 
		{
			++m_LoopDepth;
		}

		~LoopDepthGuard()
		{
			--m_LoopDepth;
		}

		LoopDepthGuard(const LoopDepthGuard&) = delete;
		LoopDepthGuard& operator=(const LoopDepthGuard&) = delete;
	};

	struct FuncDepthGuard
	{
	private:
		size_t& m_FuncDepth;

	public:
		explicit FuncDepthGuard(size_t& funcDepth)
			: m_FuncDepth{ funcDepth }
		{
			++m_FuncDepth;
		}

		~FuncDepthGuard()
		{
			--m_FuncDepth;
		}

		FuncDepthGuard(const FuncDepthGuard&) = delete;
		FuncDepthGuard& operator=(const FuncDepthGuard&) = delete;
	};

	Parser::Parser(Tokens tokens, DiagnosticEngine& diagnosticEngine, const std::string& srcName)
		: m_Tokens{ std::move(tokens) }, m_DiagnosticEngine{ diagnosticEngine }, m_SourceName{ srcName }
	{
	}

	std::vector<Ref<Stmt>> Parser::Parse()
	{
		std::vector<Ref<Stmt>> statements{};

		while (!isAtEnd())
		{
			try 
			{
				statements.push_back(declaration());
			}

			catch (const ParseError&)
			{
				synchronize();
			}
		}

		return statements;
	}

	//defining functions for grammar rules for the parser
	Ref<Expr> Parser::expression()
	{
		return assignment();
	}

	Ref<Expr> Parser::assignment()
	{
		Ref<Expr> expr{ logicalOr() };

		if (match(TokenType::EQUAL, TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL, TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL))
		{
			Token op{ previous() };
			Token binaryOp{};

			switch (op.m_Type)
			{
				case TokenType::EQUAL:											  break;
				case TokenType::PLUS_EQUAL  : binaryOp.m_Type = TokenType::PLUS;  break;
				case TokenType::MINUS_EQUAL : binaryOp.m_Type = TokenType::MINUS; break;
				case TokenType::STAR_EQUAL  : binaryOp.m_Type = TokenType::STAR;  break;
				case TokenType::SLASH_EQUAL : binaryOp.m_Type = TokenType::SLASH; break;
			}

			Ref<Expr> rightExpr{ assignment() };

			VariableExpr* variableExpr{ dynamic_cast<VariableExpr*>(expr.get()) };
			GetExpr* getExpr{ dynamic_cast<GetExpr*>(expr.get()) };
			if (variableExpr)
			{
				Token name { variableExpr->m_Token };
				if (op.m_Type != TokenType::EQUAL)
				{
					Ref<Expr> binaryExpr{ CreateRef<BinaryExpr>(CreateRef<VariableExpr>(std::move(name)), std::move(binaryOp), rightExpr) };
					return CreateRef<AssignExpr>(std::move(name), binaryExpr);
				}
				return CreateRef<AssignExpr>(std::move(name), rightExpr);
			}
			else if (getExpr)
			{
				return CreateRef<SetExpr>(getExpr->m_Object, getExpr->m_Identifier, rightExpr);
			}

			throw error(op, "Invalid Assignment target.");
		}

		return expr;
	}

	Ref<Expr> Parser::logicalOr()
	{
		Ref<Expr> expr{ logicalAnd() };

		while (check(TokenType::OR))
		{
			Token op{ advance() };

			Ref<Expr> rightExpr{ logicalAnd() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);
		}

		return expr;
	}

	Ref<Expr> Parser::logicalAnd()
	{
		Ref<Expr> expr{ equality() };

		while (check(TokenType::AND))
		{
			Token op{ advance() };

			Ref<Expr> rightExpr{ equality() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);
		}

		return expr;
	}

	Ref<Expr> Parser::equality()
	{

		Ref<Expr> expr{ comparison() };

		while (check(TokenType::BANG_EQUAL) || check(TokenType::EQUAL_EQUAL))
		{

			Token op{ advance() };

			Ref<Expr> rightExpr{ comparison() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);

		}

		return expr;
	}

	Ref<Expr> Parser::comparison()
	{
		Ref<Expr> expr{ term() };

		while (check(TokenType::GREATER_EQUAL) || check(TokenType::LESS_EQUAL) || check(TokenType::GREATER) || check(TokenType::LESS))
		{

			Token op{ advance() };

			Ref<Expr> rightExpr{ term() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);
		}

		return expr;
	}

	Ref<Expr> Parser::term()
	{
		Ref<Expr> expr{ factor() };

		while (check(TokenType::PLUS) || check(TokenType::MINUS))
		{
			Token op{ advance() };

			Ref<Expr> rightExpr{ factor() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);
		}

		return expr;
	}

	Ref<Expr> Parser::factor()
	{
		Ref<Expr> expr{ unary() };

		while (check(TokenType::STAR) || check(TokenType::SLASH))
		{
			Token op{ advance() };

			Ref<Expr> rightExpr{ unary() };

			expr = CreateRef<BinaryExpr>(expr, std::move(op), rightExpr);
		}

		return expr;
	}

	Ref<Expr> Parser::unary()
	{
		Token op{ peek() };

		if (check(TokenType::BANG) || check(TokenType::MINUS))
		{
			advance();
			Ref<Expr> rightExpr{ unary() };
			return CreateRef<UnaryExpr>(std::move(op), rightExpr);
		}

		return call();
	}

	Ref<Expr> Parser::call()
	{
		Ref<Expr> expr{ primary() }; //this is the callee expression we are expecting; or to be more precise, the identifier of the function or class

		while(true)
		{
			if (match(TokenType::LEFT_PAREN))
				expr = finishCall(expr);
			else if (match(TokenType::DOT))
			{
				//for now only properties;, name is the property name;
				Token name{ consume(TokenType::IDENTIFIER, "Expected property name after '.'.") };
				expr = CreateRef<GetExpr>(expr, std::move(name));
			}
			else
				break;
		}

		return expr;
	}

	Ref<Expr> Parser::finishCall(Ref<Expr> callee)
	{
		std::vector<Ref<Expr>> arguments{};

		while (!check(TokenType::RIGHT_PAREN)) 
		{
			arguments.push_back(expression());
			if (!check(TokenType::RIGHT_PAREN)) consume(TokenType::COMMA, "Expected ',' after argument expression.");
		}

		Token closingParen{ consume(TokenType::RIGHT_PAREN, "Expected ')' after function call.") };

		return CreateRef<CallExpr>(callee, std::move(arguments), std::move(closingParen));
	}

	Ref<Expr> Parser::primary()
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
			case TokenType::FALSE : return CreateRef<LiteralExpr>(std::move(token.m_Literal));
			case TokenType::IDENTIFIER: return CreateRef<VariableExpr>(std::move(token));
			case TokenType::THIS: return CreateRef<ThisExpr>(std::move(token));
			case TokenType::SUPER: {
				consume(TokenType::DOT, "Expected '.' after super.");
				Token identifier{ consume(TokenType::IDENTIFIER, "Expected Identifier after super.") };
				return CreateRef<SuperExpr>(std::move(token), std::move(identifier));
			}
			
			case TokenType::LEFT_PAREN:
			{
				Ref<Expr> expr{ expression() };
				if (!check(TokenType::RIGHT_PAREN)) {
					throw error(peek(), "Expected ')' after Expression.");
				}

				advance();
				return CreateRef<GroupingExpr>(expr);
			}
			default:
				throw error(token, "Expected an Expression.");
		}

		return {};
	}
	
	//parsing statements
	Ref<Stmt> Parser::statement()
	{
		Token token{ peek() };

		switch (token.m_Type)
		{
			case TokenType::IF: return ifStatement();
			case TokenType::PRINT: return printStatement();
			case TokenType::WHILE: return whileStatement();
			case TokenType::FOR: return forStatement();
			case TokenType::BREAK: return breakStatement();
			case TokenType::CONTINUE: return continueStatement();
			case TokenType::RETURN: return returnStatement();
			case TokenType::LEFT_BRACE: return CreateRef<BlockStmt>(block());
		}

		return expressionStatement();
	}

	Ref<Stmt> Parser::declaration()
	{
		Token token{ peek() };

		switch (token.m_Type)
		{
			case TokenType::CLASS: return classDeclaraton();
			case TokenType::VAR: return varDeclaration();
			case TokenType::FUNC: return functionDeclaration("function");
		}

		return statement();
	}

	Ref<Stmt> Parser::expressionStatement()
	{
		Ref<Expr> expr{ expression() };
		consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateRef<ExpressionStmt>(expr);
	}

	Ref<Stmt> Parser::ifStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after if.");
		Ref<Expr> condition{ expression() };
		consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

		Ref<Stmt> thenBranch{ statement() };
		Ref<Stmt> elseBranch{ nullptr };

		if (match(TokenType::ELSE))
			elseBranch = statement();

		return CreateRef<IfStmt>(condition, thenBranch, elseBranch);
	}

	Ref<Stmt> Parser::printStatement()
	{
		advance();

		Ref<Expr> expr{ expression() };
		consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateRef<PrintStmt>(expr);
	}

	Ref<Stmt> Parser::whileStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after while.");
		Ref<Expr> condition{ expression() };
		consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition.");

		LoopDepthGuard loopdepthGuard{ m_LoopDepth };

		Ref<Stmt> body{ statement() };

		return CreateRef<WhileStmt>(condition, body);
	}

	Ref<Stmt> Parser::forStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after for.");
		Ref<Stmt> initializer{ nullptr };
		if (check(TokenType::VAR))
			initializer = varDeclaration(false);
		else if (check(TokenType::SEMICOLON))
			initializer = nullptr;
		else
			initializer = CreateRef<ExpressionStmt>(expression());

		consume(TokenType::SEMICOLON, "Expected ';' after initializer.");

		Ref<Expr> condition{ nullptr };
		if (!check(TokenType::SEMICOLON))
			condition = expression();

		consume(TokenType::SEMICOLON, "Expected ';' after condition.");

		Ref<Expr> increment{ nullptr };
		if (!check(TokenType::RIGHT_PAREN))
			increment = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after for condition.");

		LoopDepthGuard loopdepthGuard{ m_LoopDepth };

		Ref<Stmt> body{ statement() };

		return CreateRef<ForStmt>(initializer, condition, increment, body);
	}

	Ref<Stmt> Parser::breakStatement()
	{
		Token token{ advance() };

		if (m_LoopDepth == 0)
			throw error(token, "Unexpected break outside of loop.");

		consume(TokenType::SEMICOLON, "Expected ';' after break.");
		return CreateRef<BreakStmt>();
	}

	Ref<Stmt> Parser::continueStatement()
	{
		Token token{ advance() };

		if (m_LoopDepth == 0)
			throw error(token, "Unexpected continue outside of loop.");

		consume(TokenType::SEMICOLON, "Expected ';' after continue.");
		return CreateRef<ContinueStmt>();
	}

	Ref<Stmt> Parser::returnStatement()
	{
		Token token{ advance() };

		if (m_FunctionDepth == 0)
			throw error(token, "Unexpected return outside of function.");

		Ref<Expr> expr{};

		if(!check(TokenType::SEMICOLON))
			expr = expression();

		consume(TokenType::SEMICOLON, "Expected ';' after return.");

		return CreateRef<ReturnStmt>(std::move(token), expr);
	}

	Ref<Stmt> Parser::varDeclaration(bool consumeSemicolon)
	{
		advance();
		Token identifier{ consume(TokenType::IDENTIFIER, "Expected an Identifier.") };

		Ref<Expr> expr{nullptr};

		if (match(TokenType::EQUAL))
		{
			expr = expression();
		}

		if(consumeSemicolon) consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateRef<VarStmt>(std::move(identifier), expr);
	}

	Ref<Stmt> Parser::classDeclaraton()
	{
		advance();

		Token identifier{ consume(TokenType::IDENTIFIER, "Expected an Identifier after class.") };

		Ref<VariableExpr> superClass{ nullptr };
		if (match(TokenType::LESS))
		{
			Token superClassName{ consume(TokenType::IDENTIFIER, "Expected a Super Class Identifier after '<'") };
			superClass = CreateRef<VariableExpr>(superClassName);
		}

		consume(TokenType::LEFT_BRACE, "Expected '{' before class body.");

		std::vector<Ref<Stmt>> methods{};

		while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
		{
			methods.push_back(functionDeclaration("method"));
		}

		consume(TokenType::RIGHT_BRACE, "Expected '}' after class body.");

		return CreateRef<ClassStmt>(std::move(identifier), superClass, std::move(methods));
	}

	Ref<Stmt> Parser::functionDeclaration(const std::string& kind)
	{
		if(kind != "method") advance();

		Token identifier{ consume(TokenType::IDENTIFIER, "Expected a " + kind + " Identifier.") };
		consume(TokenType::LEFT_PAREN, "Expected an '(' after Identifier.");

		std::vector<Token> parameters{};
		while (check(TokenType::IDENTIFIER))
		{	
			parameters.push_back(advance());
			if(!check(TokenType::RIGHT_PAREN)) consume(TokenType::COMMA, "Expected ',' after parameter names.");
		}

		consume(TokenType::RIGHT_PAREN, "Expected an ')' after func decl.");

		std::vector<Ref<Stmt>> funcBodyStmts{};
		Token bodyToken{ peek() };
		if (!check(TokenType::LEFT_BRACE))
			throw error(bodyToken, "Expected func body.");

		FuncDepthGuard funcDepthGuard{ m_FunctionDepth };

		funcBodyStmts = block();

		Ref<BlockStmt> funcBody{ CreateRef<BlockStmt>(std::move(funcBodyStmts)) };

		return CreateRef<FunctionStmt>(std::move(identifier), funcBody, std::move(parameters));

	}
	
	std::vector<Ref<Stmt>> Parser::block()
	{
		advance(); //consuming the '{' left brace;

		std::vector<Ref<Stmt>> statements{};

		while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
		{
			statements.push_back(declaration());
		}
		
		consume(TokenType::RIGHT_BRACE, "Expected '}' after Block.");

		return statements;
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

	Token Parser::consume(TokenType type, const std::string& msg)
	{
		if (check(type))
			return advance();

		throw error(peek(), msg);
	}

	//checking the token type of the next boutta be consumed token
	bool Parser::check(TokenType type) const
	{
		if (isAtEnd()) return false;

		return peek().m_Type == type;
	}

	bool Parser::isAtEnd() const
	{
		return peek().m_Type == TokenType::END_OF_FILE;
	}

	//error handling
	Parser::ParseError Parser::error(const Token& token, const std::string& message)
	{

		Diagnostic diag{
			DiagnosticSeverity::ERROR,
			DiagnosticPhase::PARSING,
			message,
			{m_SourceName, token.m_Line, token.m_Column, 0}
		};

		m_DiagnosticEngine.Report(std::move(diag));

		return ParseError{};
	}

	void Parser::synchronize()
	{

		while (!isAtEnd())
		{

			const Token& token{ peek() };

			switch(token.m_Type)
			{
				//statement end;
				case TokenType::SEMICOLON: advance(); return;

				//statement sync points
				case TokenType::PRINT: return;
				case TokenType::RETURN: return;
				case TokenType::VAR: return;
				case TokenType::CLASS: return;
				case TokenType::FUNC: return;
				case TokenType::IF: return;
				case TokenType::WHILE: return;
				case TokenType::FOR: return;

				default: advance();
			}
		}
	}
}