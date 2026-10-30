#include "dpch.h"

#if 0

#include "AST/AstPrinter.h"

int main(int argc, char** argv)
{

	auto left_literal{ DXX::CreateScope<DXX::LiteralExpr>(5.0f) };
	auto right_literal{ DXX::CreateScope<DXX::LiteralExpr>(10.0f) };

	DXX::Token plusToken{ DXX::TokenType::PLUS, "+", DXX::Nil{}, 0 };

	auto binary_expr{ DXX::CreateScope<DXX::BinaryExpr>(
		std::move(left_literal),
		std::move(plusToken),
		std::move(right_literal)
	) };

	DXX::AstPrinter astPrinter{};
	std::println("{0}", astPrinter.Print(*binary_expr));

	return 0;
}

#endif