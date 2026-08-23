#include "dpch.h"
#include "Tokens.h"

namespace DXX {

	Tokens Tokens::Tokenizer(const std::string& sourceCode)
	{
		std::stringstream ss{ sourceCode };
		Tokens tokens{};

		char ch;

		while (ss >> ch) {
			std::string token{ ch };

			if (punctuationChars.find(ch) != std::string::npos)
			{
				tokens << token;
			}
			else if (operatorChars.find(ch) != std::string::npos)
			{
				char next{ static_cast<char>(ss.peek()) };

				if (next == '=') {
					token += static_cast<char>(ss.get());
				}

				tokens << token;
			}

			else{
				while (ss.peek() != EOF && !isspace(ss.peek()) 
						&& punctuationChars.find(static_cast<char>(ss.peek())) == std::string::npos
						&& operatorChars.find(static_cast<char>(ss.peek())) == std::string::npos) {

					ss >> ch;
					token += ch;

				}

				tokens << token;
			}
				
		}

		return tokens;
	}

}