#include "dpch.h"
#include "D++.h"

#include "Tokenizer/Tokenization.h"
#include "Parser/Parser.h"

#include "AST/AstPrinter.h"

namespace DXX {

	DXX::DXX(int argc, const std::vector<std::string>& argv)
		: m_ArgC(argc), m_ArgV(argv)
	{
	}

	DXX::~DXX()
	{
	}

	void DXX::launch()
	{	
		std::println("---------------------------------");
		std::println("Using D++ Interpreter v1.0 Beta");
		std::println("---------------------------------\n");

		if (m_ArgC > 2) {
			std::println("Usage: D++ [path to script]");
		}
		else if (m_ArgC == 2) {
			std::string filePath{ m_ArgV.at(1) };
			runFile(filePath);
		}
		else {
			runPrompt();
		}

	}

	void DXX::runFile(const std::string& filePath)
	{
		std::string sourceCode{ parseFile(filePath) };
		run(sourceCode);
	}

	void DXX::runPrompt()
	{
		while(true)
		{
			std::print(">>> ");
			std::string line{};
			if (!std::getline(std::cin, line)) {
				std::println("\nExiting D++ Interpreter!");
				break;
			}
			run(line);
			s_HadError = false;
		}
	}

	void DXX::run(const std::string& srcLine)
	{
		if (s_HadError) std::exit(65);

		Tokens tokens{ Tokenizer::Tokenize(srcLine) };

		//creating the parser
		Parser parser{ std::move(tokens) };
		Scope<Expr> expression{ parser.Parse() };

		AstPrinter astPrinter{};
		std::println("{0}", astPrinter.Print(*expression));

		std::println();
	}

	//helpers
	std::string DXX::parseFile(const std::filesystem::path& filePath)
	{
		std::error_code ec{};
		auto size{ std::filesystem::file_size(filePath, ec) };

		if (ec) {
			return {};
		}

		std::ifstream file{ filePath, std::ios::in | std::ios::binary };
		if (!file) {
			return {};
		}

		std::string sourceCode{};
		sourceCode.resize(size);

		file.read(sourceCode.data(), size);

		file.close();
		return sourceCode;
	}

}