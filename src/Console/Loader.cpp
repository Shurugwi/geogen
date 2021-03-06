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

#include <iomanip>
#include <queue>

#include "Loader.hpp"
#include <GeoGen/GeoGen.hpp>
#include "ConsoleUtils.hpp"
#include "SignalHandler.hpp"
#include "ImageWriter.hpp"
#include "loader_commands/CodeLoaderCommand.hpp"
#include "loader_commands/DebugLoaderCommand.hpp"
#include "loader_commands/GenTilesLoaderCommand.hpp"
#include "loader_commands/HelpLoaderCommand.hpp"
#include "loader_commands/LoadLoaderCommand.hpp"
#include "loader_commands/MapSizeLoaderCommand.hpp"
#include "loader_commands/ParameterLoaderCommand.hpp"
#include "loader_commands/QuitLoaderCommand.hpp"
#include "loader_commands/RandomSeedLoaderCommand.hpp"
#include "loader_commands/ReloadLoaderCommand.hpp"
#include "loader_commands/ReloadAndRunLoaderCommand.hpp"
#include "loader_commands/RenderOriginLoaderCommand.hpp"
#include "loader_commands/RenderScaleLoaderCommand.hpp"
#include "loader_commands/RenderSizeLoaderCommand.hpp"
#include "loader_commands/RunLoaderCommand.hpp"
#include "loader_commands/ProfileLoaderCommand.hpp"

using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace console;
using namespace corelib;
using namespace renderer;
using namespace utils;
using namespace std;
using namespace loader_commands;
using namespace instructions;

Loader::Loader(geogen::IStream& in, geogen::OStream& out, ProgramArguments programArguments)
: currentFile(programArguments.inputFile), outputDirectory(programArguments.outputDirectory), debug(debug), in(in), out(out), randomSeed(programArguments.seed), renderOrigin(0, 0), renderSize(MAP_SIZE_AUTOMATIC, MAP_SIZE_AUTOMATIC), mapSize(MAP_SIZE_AUTOMATIC, MAP_SIZE_AUTOMATIC), renderScale(1), isInteractive(!programArguments.isNonInteractive), parameterValues(programArguments.scriptArgumentsStrings), compiledScript(NULL)
{
	this->commandTable.AddCommand(new CodeLoaderCommand());
	this->commandTable.AddCommand(new DebugLoaderCommand());
	this->commandTable.AddCommand(new GenTilesLoaderCommand());
	this->commandTable.AddCommand(new HelpLoaderCommand());
	this->commandTable.AddCommand(new LoadLoaderCommand());
	this->commandTable.AddCommand(new MapSizeLoaderCommand());
	this->commandTable.AddCommand(new ParameterLoaderCommand());
	this->commandTable.AddCommand(new QuitLoaderCommand());
	this->commandTable.AddCommand(new RandomSeedLoaderCommand());
	this->commandTable.AddCommand(new ReloadLoaderCommand());
	this->commandTable.AddCommand(new ReloadAndRunLoaderCommand());
	this->commandTable.AddCommand(new RenderOriginLoaderCommand());
	this->commandTable.AddCommand(new RenderScaleLoaderCommand());
	this->commandTable.AddCommand(new RenderSizeLoaderCommand());
	this->commandTable.AddCommand(new RunLoaderCommand());
	this->commandTable.AddCommand(new ProfileLoaderCommand());
}

void Loader::Run()
{	
	queue<String>& commandQueue = this->GetCommandQueue();

	if (this->currentFile != GG_STR(""))
	{
		commandQueue.push(GG_STR("load ") + this->currentFile);

		if (!this->isInteractive)
		{
			commandQueue.push(GG_STR("run"));
		}
	}
	else if (!this->isInteractive)
	{
		out << GG_STR("Input script not specified.") << endl << endl;
		return;
	}

	if (this->isInteractive)
	{
		out << GG_STR("Welcome to GeoGen interactive console.") << endl << endl;
		out << GG_STR("Enter \"?\" to list commands available in current context. ") << std::endl << GG_STR("Press CTRL+C to abort most running operations.") << endl << endl;
	}

	String input = "";
	while (true)
	{
		if (commandQueue.empty())
		{
			if (!this->isInteractive)
			{
				break;
			}

			out << GG_STR("LOADER>> ");
			if (!getline<Char>(in, input)){
				IgnoreNextSignal();
				out << std::endl;
				return; 
			}

			in.clear();
		}
		else
		{
			input = commandQueue.front();
			commandQueue.pop();
		}

		size_t separatorPosition = input.find(" ");
		string commandCue = input.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != String::npos)
		{
			args = input.substr(separatorPosition + 1);
		}

		Command const* command = this->commandTable.GetCommand(commandCue);
		if (command == NULL)
		{
			//if (GetAndClearAbortFlag()) return;

			out << GG_STR("Unknown command. Enter \"?\" to print list of commands available in current context.") << endl << endl;
		}
		else
		{
			try
			{
				dynamic_cast<LoaderCommand const*>(command)->Run(this, args);
			}
			catch (GeoGenException& e)
			{
				// Empty the command queue (the commands likely depended on the current command succeeding)
				while (!commandQueue.empty()) commandQueue.pop();

				HighlightRed();
				out << "Error GGE" << e.GetErrorCode() << ": " << e.GetDetailMessage() << endl << endl;
				Unhighlight();

				if (this->GetDump() != GG_STR("") && this->isInteractive)
				{
					out << GG_STR("Do you wish to save dump file? (y/n, default = n) ");
					String saveDumpResponse;
					if (!getline(Cin, saveDumpResponse))
					{
						cin.clear();
						IgnoreNextSignal();
						out << GG_STR("Discarded dump file.") << std::endl << std::endl;
						this->SetDump(GG_STR(""));
						continue;
					}

					cin.clear();

					if (saveDumpResponse == GG_STR("y") || saveDumpResponse == GG_STR("Y") || saveDumpResponse == GG_STR("yes"))
					{
						out << GG_STR("Dump file name (default = dump.txt): ");
						String dumpFileName;
						if (!getline(Cin, dumpFileName))
						{
							cin.clear();
							IgnoreNextSignal();
							out << GG_STR("Discarded dump file.") << std::endl << std::endl;
							this->SetDump(GG_STR(""));
							continue;
						}

						cin.clear();

						if (dumpFileName == GG_STR(""))
						{
							dumpFileName = "dump.txt";
						}

						dumpFileName = this->outputDirectory + GG_STR("/") + dumpFileName;

						OFStream dumpStream(dumpFileName.c_str());
						dumpStream << this->GetDump();
						dumpStream.flush();

						if (dumpStream.fail())
						{
							out << GG_STR("Could not write dump file.") << std::endl << std::endl;
						}
						else
						{
							out << GG_STR("Saved dump file \"") << dumpFileName << GG_STR("\".") << std::endl << std::endl;
						}
					}
					else
					{
						out << GG_STR("Discarded dump file.") << std::endl << std::endl;
					}

					this->SetDump(GG_STR(""));
				}
			}
		}
	}
}

bool Loader::SaveRenderedMaps(renderer::RenderedMapTable& renderedMaps, String prefix)
{
	for (RenderedMapTable::iterator it = renderedMaps.Begin(); it != renderedMaps.End(); it++)
	{
		if (GetAndClearAbortFlag())
		{
			out << GG_STR("Aborted.") << endl << endl;
			return false;
		}

		stringstream ss;
		ss << this->outputDirectory << GG_STR("/") << prefix << StringToAscii(it->first);

		bool success = true;
		try
		{
			WriteImage(it->second, RENDERER_OBJECT_TYPE_HEIGHT_MAP, ss.str());
		}
		catch (exception&)
		{
			success = false;
			out << GG_STR("Could not save \"") << ss.str() << GG_STR("\".") << endl;
		}

		if (success)
		{
			out << GG_STR("Saved \"") << ss.str() << "\"." << endl;
		}		
	}
	renderedMaps.Clear();

	out << endl;

	return true;
}

ScriptParameters Loader::CreateScriptParameters()
{
	ScriptParameters params = this->GetCompiledScript()->CreateScriptParameters();

	params.SetRenderOriginX(this->GetRenderOrigin().GetX());
	params.SetRenderOriginY(this->GetRenderOrigin().GetY());
	params.SetRenderWidth(this->GetRenderSize().GetWidth());
	params.SetRenderHeight(this->GetRenderSize().GetHeight());
	params.SetMapWidth(this->GetMapSize().GetWidth());
	params.SetMapHeight(this->GetMapSize().GetHeight());
	params.SetRenderScale(this->GetRenderScale());
	params.SetRandomSeed(random::RecognizeSeed(this->randomSeed));

	for (ScriptParameters::iterator it = params.Begin(); it != params.End(); it++)
	{
		if (this->parameterValues.find(it->first) != this->parameterValues.end())
		{
			String stringValue = this->parameterValues[it->first];

			if (it->second->GetType() == SCRIPT_PARAMETER_TYPE_BOOLEAN)
			{
				BooleanScriptParameter* boolParameter = dynamic_cast<BooleanScriptParameter*>(it->second);
				if (stringValue == GG_STR("true") || stringValue == GG_STR("True") || stringValue == GG_STR("t") || stringValue == GG_STR("T") || stringValue == GG_STR("1") || stringValue == GG_STR("on") || stringValue == GG_STR("On") || stringValue == GG_STR("yes") || stringValue == GG_STR("Yes") || stringValue == GG_STR("y") || stringValue == GG_STR("Y"))
				{
					boolParameter->SetValue(true);
				}
				else if (stringValue == GG_STR("false") || stringValue == GG_STR("False")|| stringValue == GG_STR("f") || stringValue == GG_STR("F") || stringValue == GG_STR("0") || stringValue == GG_STR("off") || stringValue == GG_STR("Off") || stringValue == GG_STR("yes") || stringValue == GG_STR("Yes") || stringValue == GG_STR("y") || stringValue == GG_STR("Y"))
				{
					boolParameter->SetValue(true);
				}
				else
				{
					this->PrintScriptParameterWarning(it->first, stringValue, ScriptParameterTypeToString(it->second->GetType()));
				}
			}
			else if (it->second->GetType() == SCRIPT_PARAMETER_TYPE_NUMBER)
			{
				Number numberValue;
				StringStream numberStream(stringValue);
				numberStream >> numberValue;
				if (numberStream.fail())
				{
					this->PrintScriptParameterWarning(it->first, stringValue, ScriptParameterTypeToString(it->second->GetType()));
				}

				NumberScriptParameter* numberParameter = dynamic_cast<NumberScriptParameter*>(it->second);
				numberParameter->SetValue(numberValue);
			}
			else if (it->second->GetType() == SCRIPT_PARAMETER_TYPE_ENUM)
			{
				EnumScriptParameter* enumParameter = dynamic_cast<EnumScriptParameter*>(it->second);

				Number numberValue;
				StringStream numberStream(stringValue);
				numberStream >> numberValue;
				int intValue = NumberToInt(numberValue);
				if (!numberStream.fail())
				{
					bool found = false;
					for (EnumTypeDefinition::ValueDefinitions::const_iterator it2 = enumParameter->GetEnumType()->GetValueDefinitions().begin(); it2 != enumParameter->GetEnumType()->GetValueDefinitions().end(); it2++)
					{
						if (it2->second == intValue){
							found = true;
							enumParameter->SetValue(intValue);
						}
					}

					if (!found)
					{
						this->PrintScriptParameterWarning(it->first, stringValue, enumParameter->GetEnumType()->GetName());
					}
				}
				else
				{
					EnumTypeDefinition::ValueDefinitions::const_iterator it2 = enumParameter->GetEnumType()->GetValueDefinitions().find(stringValue);

					if (it2 == enumParameter->GetEnumType()->GetValueDefinitions().end())
					{
						this->PrintScriptParameterWarning(it->first, stringValue, enumParameter->GetEnumType()->GetName());
					}
					else 
					{
						enumParameter->SetValue(it2->second);
					}
				}
			}
			else throw InternalErrorException(GG_STR("Invalid ScriptParameterType."));
		}
	}

	return params;
}

void Loader::PrintScriptParameterWarning(String name, String value, String type)
{
	this->GetOut() << GG_STR("WARNING: Value \"") << value << GG_STR("\" of script parameter \"") << name << GG_STR("\" could not be parsed as type ") << type << GG_STR(".") << std::endl;
}