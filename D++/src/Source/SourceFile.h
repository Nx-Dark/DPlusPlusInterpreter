#pragma once

#include <string>
#include <sstream>
#include <cstddef>

namespace DXX
{

	class SourceFile
	{
	private:

		std::string m_Name{};
		std::string m_Contents{};

	public:
		SourceFile(const std::string& name, const std::string& contents)
			: m_Name{name}, m_Contents{contents} { }

		inline const std::string& GetName() const { return m_Name; }
		
		inline const std::string& GetContents() const { return m_Contents; }
		
		inline std::string GetLine(std::size_t _line) const
		{
			//return empty string is line is 0 or less;
			if (_line < 1) return "";

			std::stringstream ss{ m_Contents };
			std::string line{};
			std::size_t currentLine{};

			while (std::getline(ss, line)) 
			{
				currentLine++;
				if (currentLine == _line)
					return line;
			}

			return "";
		}

	};

}