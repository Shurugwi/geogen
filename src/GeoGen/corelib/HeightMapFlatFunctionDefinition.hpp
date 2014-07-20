#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Flat. </summary>
		class HeightMapFlatFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapFlatFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Flat"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}