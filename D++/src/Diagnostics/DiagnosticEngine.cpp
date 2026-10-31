#include "dpch.h"
#include "DiagnosticEngine.h"

namespace DXX 
{

	DiagnosticEngine::DiagnosticEngine()
	{
		m_Diagnostics.reserve(10);
	}

	void DiagnosticEngine::Report(Diagnostic diagnostic)
	{
		m_Diagnostics.push_back(std::move(diagnostic));
	}

	bool DiagnosticEngine::HasErrors() const
	{
		for (const auto& diag : m_Diagnostics)
		{
			if (diag.severity == DiagnosticSeverity::ERROR)
				return true;
		}

		return false;
	}

	const std::vector<Diagnostic>& DiagnosticEngine::GetDiagnostics() const
	{
		return m_Diagnostics;
	}

	void DiagnosticEngine::Clear()
	{
		m_Diagnostics.clear();
	}

}