#include "dpch.h"
#include "Application.h"

#include "Tokenizer/Tokenization.h"
#include "Parser/Parser.h"

#include "AST/AstPrinter.h"

namespace DXX {

	Application::Application(int argc, const std::vector<std::string>& argv)
		: m_ArgC{ argc }, m_ArgV{ argv }, m_DiagnosticReporter{ m_DiagnosticEngine, m_SourceManager }
	{
	}

	Application::~Application()
	{
	}

	void Application::launch()
	{	
		std::println("---------------------------------");
		std::println("Using D++ Interpreter v{0} {1}", VERSION, BUILD_TYPE);
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

	void Application::runFile(const std::string& filePath)
	{
		std::string sourceCode{ parseFile(filePath) };
		std::println("{0}", filePath);
		run(sourceCode, filePath);
	}

	void Application::runPrompt()
	{
		while(true)
		{
			std::print(">>> ");
			std::string line{};
			if (!std::getline(std::cin, line)) {
				std::println("\nExiting D++ Interpreter!");
				break;
			}
			run(line, "InteractiveMode");
		}
	}

	void Application::run(const std::string& sourceCode, const std::string& sourceName)
	{
		//clear diagnostics data;
		m_DiagnosticEngine.Clear();
		
		m_SourceManager.AddSource(sourceName, sourceCode);
		Tokens tokens{ Tokenizer::Tokenize(sourceCode, sourceName, m_DiagnosticEngine) };

		//creating the parser
		Parser parser{ std::move(tokens), m_DiagnosticEngine, sourceName };
		Scope<Expr> expression{ parser.Parse() };
		
		//evalution and interpreting does here(future plans);

		//reporting diagnostic data;
		m_DiagnosticReporter.ReportDiagnostics();

		if (expression)
		{
			AstPrinter astPrinter{};
			std::println("{0}", astPrinter.Print(*expression));
		}

		//for (const auto& token : tokens)
		//{
		//	std::println("{0}", token.toString());
		//}

		std::println();
	}

	//helpers
	std::string Application::parseFile(const std::filesystem::path& filePath)
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