#pragma once

#include "DiagnosticEngine.h"
#include "Source/SourceManager.h"

namespace DXX
{

	class DiagnosticReporter
	{

	private:

		DiagnosticEngine& m_DiagnosticEngine;
		SourceManager& m_SourceManager;

	public:
		DiagnosticReporter(DiagnosticEngine& diagnosticEngine, SourceManager& sourceManager);

		void ReportDiagnostics();

	};

}