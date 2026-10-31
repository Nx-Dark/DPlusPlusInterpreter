#pragma once

#include "Diagnostic.h"

namespace DXX
{

	class DiagnosticEngine
	{
	private:

		std::vector<Diagnostic> m_Diagnostics{};

	public:
		DiagnosticEngine();

		void Report(Diagnostic diagnostic);

		bool HasErrors() const;

		const std::vector<Diagnostic>& GetDiagnostics() const;

		void Clear();



	};

}