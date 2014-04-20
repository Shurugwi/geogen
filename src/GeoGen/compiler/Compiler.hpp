#pragma once

#include <string>
#include <vector>

#include "../runtime/CompiledScript.hpp"
#include "../runtime/VariableDefinition.hpp"
#include "../runtime/FunctionDefinition.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../runtime/SymbolDefinitionTable.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class Compiler
		{
			private:
				Compiler(const Compiler&); // Not copyable
				void CreateScriptParameters(runtime::CompiledScript& code) const;
			public:
				Compiler(); 
				runtime::CompiledScript* CompileScript(std::string const& code) const;
		};
	}
}