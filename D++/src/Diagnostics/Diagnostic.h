#pragma once

#include <string>
#include <cstddef>

namespace DXX
{

	//error severity
	enum class DiagnosticSeverity
	{
		INFO,
		WARNING,
		ERROR
	};

	//error phase
	enum class DiagnosticPhase
	{
		LEXING,
		PARSING,
		RESOLVING,
		RUNTIME
	};

	//source of the error
	struct SourceLocation 
	{
		std::string sourceName{}; //name of the file or the input which produced a error
		std::size_t line{};
		std::size_t column{};
		std::size_t length{};
	};

	//diagnostic report
	struct Diagnostic
	{
		DiagnosticSeverity severity{};
		DiagnosticPhase phase{};

		std::string message{};
		SourceLocation location{}; //data represting the source file name and its details of where the error occured;
	};

}