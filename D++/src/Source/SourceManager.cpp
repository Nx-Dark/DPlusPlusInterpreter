#include "dpch.h"
#include "SourceManager.h"

namespace DXX
{

	void SourceManager::AddSource(const std::string& name, const std::string& contents)
	{
		
		for (auto& srcFile : m_SourceFiles)
		{
			if (srcFile.GetName() == name)
			{
				srcFile = SourceFile{ name, contents };
				return;
			}
		}

		m_SourceFiles.emplace_back(name, contents);
	}

	const SourceFile* SourceManager::GetSourceFile(const std::string& name) const
	{
		for (const auto& srcFile : m_SourceFiles)
		{
			if (srcFile.GetName() == name)
				return &srcFile;
		}

		return nullptr;
	}

}