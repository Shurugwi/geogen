/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <vector>
#include <map>
#include <queue>

#include <GeoGen/GeoGen.hpp>
#include "CommandTable.hpp"
#include "ProgramArguments.hpp"


namespace geogen
{
	namespace console
	{
		class Loader
		{
			compiler::Compiler compiler;
			CommandTable commandTable;

			geogen::IStream& in;
			geogen::OStream& out;

			bool debug;
			String currentFile;
			String outputDirectory;
			runtime::CompiledScript* compiledScript = NULL;
			//String code;
			String dump;
			bool isInteractive;

			Point renderOrigin;
			Size2D renderSize;
			Size2D mapSize;
			Scale renderScale;

			String randomSeed;

			std::map<String, String> parameterValues;

			std::queue<String> commandQueue;
		public:
			Loader(geogen::IStream& in, geogen::OStream& out, ProgramArguments parameters);
			~Loader() { if (compiledScript != NULL) delete compiledScript; };

			inline geogen::IStream& GetIn() { return this->in; }
			inline geogen::OStream& GetOut() { return this->out; }

			inline compiler::Compiler* GetCompiler() { return &this->compiler; };
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			inline String GetOutputDirectory() const { return this->outputDirectory; }
			inline void SetOutputDirectory(String outputDirectory) { this->outputDirectory = outputDirectory; }

			inline String GetDump() const { return this->dump; }
			inline void SetDump(String dump) { this->dump = dump; }

			inline bool IsInDebugMode() const { return this->debug; }
			inline void SetDebugMode(bool debugMode) { this->debug = debug; }

			inline runtime::CompiledScript* GetCompiledScript() const { return this->compiledScript; }
			inline void SetCompiledScript(String fileName, runtime::CompiledScript* compiledScript) 
			{ 
				if (this->compiledScript != NULL) { delete this->compiledScript; } 
				this->compiledScript = compiledScript; 
				this->currentFile = fileName; 
			}

			inline String GetCurrentFileName() const { return this->currentFile; }

			inline Point GetRenderOrigin() const { return this->renderOrigin; }
			inline void SetRenderOrigin(Point renderOrigin) { this->renderOrigin = renderOrigin; }

			inline Size2D GetRenderSize() const { return this->renderSize; }
			inline void SetRenderSize(Size2D renderSize) { this->renderSize = renderSize; }

			inline Size2D GetMapSize() const { return this->mapSize; }
			inline void SetMapSize(Size2D mapSize) { this->mapSize = mapSize; }

			inline Scale GetRenderScale() const { return this->renderScale; }
			inline void SetRenderScale(Scale renderScale) { this->renderScale = renderScale; }

			inline String GetRandomSeed() const { return this->randomSeed; }
			inline void SetRandomSeed(String randomSeed) { this->randomSeed = randomSeed; }

			inline std::map<String, String>& GetParameterValues() { return this->parameterValues; }

			inline bool IsInteractive()const { return this->isInteractive; }

			inline std::queue<String>& GetCommandQueue() { return this->commandQueue; }

			geogen::runtime::ScriptParameters CreateScriptParameters();

			void Run();
			bool SaveRenderedMaps(renderer::RenderedMapTable& renderedMaps, String prefix = GG_STR(""));

			void PrintScriptParameterWarning(String name, String value, String type);
		};
	}
}