#pragma once

#ifndef VERSION
#define VERSION 1.1f
#endif

#ifndef BUILD_TYPE
#define BUILD_TYPE "BETA"
#endif

#include "Diagnostics/DiagnosticEngine.h"
#include "Diagnostics/DiagnosticReporter.h"

namespace DXX {

	class Application
	{
	private:

		int m_ArgC{};
		std::vector<std::string> m_ArgV{};

		//source management
		SourceManager m_SourceManager;

		//diagnostics and error reporting
		DiagnosticEngine m_DiagnosticEngine;
		DiagnosticReporter m_DiagnosticReporter;

	public:
		Application(int argc, const std::vector<std::string>& argv);
		~Application();

		void launch();

	private:
		void runFile(const std::string& filePath);
		void runPrompt();

		void run(const std::string& sourceCode, const std::string& sourceName);

		//parsing file helper function
		std::string parseFile(const std::filesystem::path& filePath);
	};

}