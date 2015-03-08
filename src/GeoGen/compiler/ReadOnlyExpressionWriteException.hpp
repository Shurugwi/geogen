#pragma once

#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1307_ReadOnlyExpressionWrite occurs.
		class ReadOnlyExpressionWriteException : public CompilerException
		{
		private:			
		public:

			/// Constructor.
			/// @param location The location.
			explicit ReadOnlyExpressionWriteException(CodeLocation location) :
				CompilerException(GGE1307_ReadOnlyExpressionWrite, location)
			{
			};

			virtual ~ReadOnlyExpressionWriteException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to write read-only expression on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}