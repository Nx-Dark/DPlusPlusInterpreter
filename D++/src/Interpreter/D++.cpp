#include "dpch.h"
#include "D++.h"

#include "Tokens.h"

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
			m_HadError = false;
		}
	}

	void DXX::run(const std::string& srcLine)
	{
		if (m_HadError) std::exit(65);

		Tokens tokens{ Tokens::Tokenizer(srcLine) };

		for (const auto& t : tokens)
			std::println("{0}", t);

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

		if (!file) {
			file.close();
			return {};
		}

		file.close();
		return sourceCode;
	}

	//error reporting
	void DXX::error(int line, const std::string& message) {
		reportError(line, "", message);
	}

	void DXX::reportError(int line, const std::string& where, const std::string& message) {
		std::println("[line: {0}] -> Error: {1}: {2}", line, where, message);
		m_HadError = true;
	}
}