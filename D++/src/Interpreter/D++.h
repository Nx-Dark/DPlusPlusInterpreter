#pragma once

namespace DXX {

	

	class DXX
	{
	private:

		int m_ArgC{};
		std::vector<std::string> m_ArgV{};

		bool m_HadError{};

	public:
		DXX(int argc, const std::vector<std::string>& argv);
		~DXX();

		void launch();

	private:
		void runFile(const std::string& filePath);
		void runPrompt();

		void run(const std::string& sourceLine);

		//parsing file helper function
		std::string parseFile(const std::filesystem::path& filePath);
		
		//error reporting
	public:
		void error(int line, const std::string& message);

	private:
		void reportError(int line, const std::string& where, const std::string& message);
	};

}