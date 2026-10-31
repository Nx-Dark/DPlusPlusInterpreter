#include "dpch.h"
#include "DiagnosticReporter.h"

namespace DXX
{

	//helper function to convert DiagnosticSeverity to String data;
	static std::string DiagnosticSeverityToString(DiagnosticSeverity severity)
	{
		switch (severity)
		{
			case DiagnosticSeverity::INFO   : return "info";
			case DiagnosticSeverity::WARNING: return "warning";
			case DiagnosticSeverity::ERROR  : return "error";
		}

		return "unknown";
	}

	static std::string DiagnosticPhaseToString(DiagnosticPhase phase)
	{
		switch (phase)
		{
			case DiagnosticPhase::LEXING   : return "lexing";
			case DiagnosticPhase::PARSING  : return "parsing";
			case DiagnosticPhase::RESOLVING: return "resolving";
			case DiagnosticPhase::RUNTIME  : return "runtime";
		}

		return "unknown";
	}

	DiagnosticReporter::DiagnosticReporter(DiagnosticEngine& diagnosticEngine, SourceManager& sourceManager)
		: m_DiagnosticEngine{ diagnosticEngine }, m_SourceManager{ sourceManager }
	{
	}

	void DiagnosticReporter::ReportDiagnostics()
	{
		//retreivng the diagnostic data from the diagnosticengine;
		const auto& diagnosticData{ m_DiagnosticEngine.GetDiagnostics() };

		for (const auto& diagnostic : diagnosticData)
		{
			const auto* sourceFile{ m_SourceManager.GetSourceFile(diagnostic.location.sourceName) };

			std::println("{0}[{1}]: {2}", DiagnosticSeverityToString(diagnostic.severity), 
				DiagnosticPhaseToString(diagnostic.phase),	diagnostic.message);

			std::println("  --> {0}:{1}:{2}", diagnostic.location.sourceName, diagnostic.location.line,
				diagnostic.location.column);

			if (sourceFile)
			{
				std::string srcLine{ sourceFile->GetLine(diagnostic.location.line) };

				std::println("  |");
				std::println("{0} | {1}", diagnostic.location.line, srcLine);
				std::println("  |");
			}
		}

	}

}