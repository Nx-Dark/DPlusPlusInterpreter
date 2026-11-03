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

	Parser::Parser(Tokens tokens, DiagnosticEngine& diagnosticEngine, const std::string& srcName)
		: m_Tokens{ std::move(tokens) }, m_DiagnosticEngine{ diagnosticEngine }, m_SourceName{ srcName }
	{
	}

	std::vector<Scope<Stmt>> Parser::Parse()
	{
		std::vector<Scope<Stmt>> statements{};

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
	Scope<Expr> Parser::expression()
	{
		return assignment();
	}

	Scope<Expr> Parser::assignment()
	{
		Scope<Expr> expr{ logicalOr() };

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

			Scope<Expr> rightExpr{ assignment() };

			VariableExpr* variableExpr{ dynamic_cast<VariableExpr*>(expr.get()) };
			if (variableExpr)
			{
				Token name { variableExpr->m_Token };
				if (op.m_Type != TokenType::EQUAL)
				{
					Scope<Expr> binaryExpr{ CreateScope<BinaryExpr>(std::move(CreateScope<VariableExpr>(name)), binaryOp, std::move(rightExpr)) };
					return CreateScope<AssignExpr>(std::move(name), std::move(binaryExpr));
				}
				return CreateScope<AssignExpr>(std::move(name), std::move(rightExpr));
			}

			throw error(op, "Invalid Assignment target.");
		}

		return expr;
	}

	Scope<Expr> Parser::logicalOr()
	{
		Scope<Expr> expr{ logicalAnd() };

		while (check(TokenType::OR))
		{
			Token op{ advance() };

			Scope<Expr> rightExpr{ logicalAnd() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));
		}

		return expr;
	}

	Scope<Expr> Parser::logicalAnd()
	{
		Scope<Expr> expr{ equality() };

		while (check(TokenType::AND))
		{
			Token op{ advance() };

			Scope<Expr> rightExpr{ equality() };

			expr = CreateScope<BinaryExpr>(std::move(expr), std::move(op), std::move(rightExpr));
		}

		return expr;
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

		return call();
	}

	Scope<Expr> Parser::call()
	{
		Scope<Expr> callee{ primary() };

		while(match(TokenType::LEFT_PAREN))
		{
			callee = finishCall(std::move(callee));
		}

		return callee;
	}

	Scope<Expr> Parser::finishCall(Scope<Expr> callee)
	{
		std::vector<Scope<Expr>> arguments{};

		while (!check(TokenType::RIGHT_PAREN)) 
		{
			arguments.push_back(expression());
			if (!check(TokenType::RIGHT_PAREN)) consume(TokenType::COMMA, "Expected ',' after argument expression.");
		}

		Token closingParen{ consume(TokenType::RIGHT_PAREN, "Expected ')' after function call.") };

		return CreateScope<CallExpr>(std::move(callee), std::move(arguments), std::move(closingParen));
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
			case TokenType::IDENTIFIER: return CreateScope<VariableExpr>(std::move(token));
			
			case TokenType::LEFT_PAREN:
			{
				Scope<Expr> expr{ expression() };
				if (!check(TokenType::RIGHT_PAREN)) {
					throw error(peek(), "Expected ')' after Expression.");
				}

				advance();
				return CreateScope<GroupingExpr>(std::move(expr));
			}
			default:
				throw error(token, "Expected an Expression.");
		}

		return {};
	}
	
	//parsing statements
	Scope<Stmt> Parser::statement()
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
			case TokenType::LEFT_BRACE: return CreateScope<BlockStmt>(std::move(block()));
		}

		return expressionStatement();
	}

	Scope<Stmt> Parser::declaration()
	{
		Token token{ peek() };

		switch (token.m_Type)
		{
			case TokenType::VAR: return varDeclaration();
			case TokenType::FUNC: return functionDeclaration();
		}

		return statement();
	}

	Scope<Stmt> Parser::expressionStatement()
	{
		Scope<Expr> expr{ expression() };
		consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateScope<ExpressionStmt>(std::move(expr));
	}

	Scope<Stmt> Parser::ifStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after if.");
		Scope<Expr> condition{ expression() };
		consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

		Scope<Stmt> thenBranch{ statement() };
		Scope<Stmt> elseBranch{ nullptr };

		if (match(TokenType::ELSE))
			elseBranch = statement();

		return CreateScope<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
	}

	Scope<Stmt> Parser::printStatement()
	{
		advance();

		Scope<Expr> expr{ expression() };
		consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateScope<PrintStmt>(std::move(expr));
	}

	Scope<Stmt> Parser::whileStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after while.");
		Scope<Expr> condition{ expression() };
		consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition.");

		LoopDepthGuard loopdepthGuard{ m_LoopDepth };

		Scope<Stmt> body{ statement() };

		return CreateScope<WhileStmt>(std::move(condition), std::move(body));
	}

	Scope<Stmt> Parser::forStatement()
	{
		advance();

		consume(TokenType::LEFT_PAREN, "Expected '(' after for.");
		Scope<Stmt> initializer{ nullptr };
		if (check(TokenType::VAR))
			initializer = varDeclaration(false);
		else if (check(TokenType::SEMICOLON))
			initializer = nullptr;
		else
			initializer = CreateScope<ExpressionStmt>(expression());

		consume(TokenType::SEMICOLON, "Expected ';' after initializer.");

		Scope<Expr> condition{ nullptr };
		if (!check(TokenType::SEMICOLON))
			condition = expression();

		consume(TokenType::SEMICOLON, "Expected ';' after condition.");

		Scope<Expr> increment{ nullptr };
		if (!check(TokenType::RIGHT_PAREN))
			increment = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after for condition.");

		LoopDepthGuard loopdepthGuard{ m_LoopDepth };

		Scope<Stmt> body{ statement() };

		return CreateScope<ForStmt>(std::move(initializer), std::move(condition), std::move(increment), std::move(body));
	}

	Scope<Stmt> Parser::breakStatement()
	{
		Token token{ advance() };

		if (m_LoopDepth == 0)
			throw error(token, "Unexpected break outside of loop.");

		consume(TokenType::SEMICOLON, "Expected ';' after break.");
		return CreateScope<BreakStmt>();
	}

	Scope<Stmt> Parser::continueStatement()
	{
		Token token{ advance() };

		if (m_LoopDepth == 0)
			throw error(token, "Unexpected continue outside of loop.");

		consume(TokenType::SEMICOLON, "Expected ';' after continue.");
		return CreateScope<ContinueStmt>();
	}

	Scope<Stmt> Parser::varDeclaration(bool consumeSemicolon)
	{
		advance();
		Token identifier{ consume(TokenType::IDENTIFIER, "Expected an Identifier.") };

		Scope<Expr> expr{nullptr};

		if (match(TokenType::EQUAL))
		{
			expr = expression();
		}

		if(consumeSemicolon) consume(TokenType::SEMICOLON, "Expected ';' after Statement.");
		return CreateScope<VarStmt>(std::move(identifier), std::move(expr));
	}

	Scope<Stmt> Parser::functionDeclaration()
	{
		advance();

		Token identifier{ consume(TokenType::IDENTIFIER, "Expected an Identifier after func.") };
		consume(TokenType::LEFT_PAREN, "Expected an '(' after Identifier.");

		std::vector<Token> parameters{};
		while (check(TokenType::IDENTIFIER))
		{
			parameters.push_back(advance());
			if(!check(TokenType::RIGHT_PAREN)) consume(TokenType::COMMA, "Expected ',' after parameter names.");
		}

		consume(TokenType::RIGHT_PAREN, "Expected an ')' after func decl.");

		std::vector<Scope<Stmt>> funcBodyStmts{};
		Token bodyToken{ peek() };
		if (!check(TokenType::LEFT_BRACE))
			throw error(bodyToken, "Expected func body.");

		funcBodyStmts = block();

		Scope<BlockStmt> funcBody{ CreateScope<BlockStmt>(std::move(funcBodyStmts)) };

		return CreateScope<FunctionStmt>(std::move(identifier), std::move(funcBody), std::move(parameters));

	}
	
	std::vector<Scope<Stmt>> Parser::block()
	{
		advance(); //consuming the '{' left brace;

		std::vector<Scope<Stmt>> statements{};

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