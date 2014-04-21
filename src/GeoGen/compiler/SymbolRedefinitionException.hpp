#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class SymbolRedefinitionException : public CompilerException
		{
		private:
			std::string symbolName;
		public:
			SymbolRedefinitionException(ErrorCode code, CodeLocation location, std::string const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Symbol \"" << symbolName << "\" defined on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " is already defined in its scope.";

				return ss.str();
			}
		};
	}
}