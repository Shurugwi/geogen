#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class ReadOnlyWriteException : public RuntimeException
		{
		private:
			std::string symbolName;
		public:
			explicit ReadOnlyWriteException(CodeLocation location, std::string const& symbolName) :
				RuntimeException(GGE2108_ReadOnlyWrite, location), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage()
			{
				return "Attempted to write read only variable \"" + symbolName + "\".";
			}
		};
	}
}