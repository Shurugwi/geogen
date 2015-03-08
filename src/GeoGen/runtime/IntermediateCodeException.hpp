
#pragma once

#include "../ErrorCode.hpp"
#include "../GeoGenException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE5001_IntermediateCodeError occurs.
		class IntermediateCodeException : public GeoGenException
		{
		private:
			String message;
		public:
			explicit IntermediateCodeException(String const& message) :
				GeoGenException(GGE5001_IntermediateCodeError) {};

			virtual ~IntermediateCodeException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Incorrect intermediate code was being executed, possibly indicating an internal error in the compiler (" << message << GG_STR(").");
				return ss.str();
			}
		};
	}
}