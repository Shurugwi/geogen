#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"
#include "VirtualMachine.hpp"

namespace geogen
{
	namespace runtime
	{
		class ReferenceTypeDefinition : public TypeDefinition
		{
		protected:
			ReferenceTypeDefinition(String const& name) : TypeDefinition(name) {}
		public:			
			virtual ManagedObject* Copy(ManagedObject* a) const;
		};
	}
}