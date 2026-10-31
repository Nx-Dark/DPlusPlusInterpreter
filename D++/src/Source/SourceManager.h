#pragma once

#include "SourceFile.h"

namespace DXX
{

	class SourceManager
	{

	private:

		std::vector<SourceFile> m_SourceFiles{};

	public:
		SourceManager() = default;

		void AddSource(const std::string& name, const std::string& contents);

		const SourceFile* GetSourceFile(const std::string& name) const;

	};

}