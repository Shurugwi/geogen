/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
//#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <time.h>

using namespace std;

#ifdef NO_GGEN_LIB
	#include "ggen.h"
	#include "ggen_squirrel.h"
#else
	#pragma comment(lib,"../lib/GeoGen.lib")
	#include "../include/geogen.h"
#endif

#include "EasyBMP.h"


int _width = 0;
int _height = 0;

bool SaveAsBMP(int16* data, int width, int height, const char* implicit_path, const char* name = NULL){
	stringstream path_out;

	if(name == NULL){
		path_out << implicit_path;
		cout << "Saving main bitmap...\n";
	}
	else{
		path_out << "../temp/" << name << ".bmp";
		cout << "Saving map \"" << name << "\"...\n";
	}
	
	BMP output;

	output.SetBitDepth(32);

	output.SetSize(width, height);

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			output(j,i)->Red = output(j,i)->Green = output(j,i)->Blue = (ebmpBYTE) data[j + width * i];
		}		
	}

	output.WriteToFile(path_out.str().c_str());

	if(name != NULL) cout << "Executing...\n";

	return true;
}

void ReturnHandler(char* name, int16* map, int width, int height){
	SaveAsBMP(map, width, height, "", name); 
}

int main(int argc,char * argv[]){

	char* path_out;
	char* path_in;
	int seed;


	// Parse arguments

	// Display help text if requested
	if(argc == 2 && (argv[1][0] == '-' || argv[1][0] == '/') && argv[1][1] == '?'){
		cout << "\n\
GeoGen - open-source procedural heightmap generator			\n\
\n\
Syntax:\n\
geogen path_to_script output_file result_width result_height\n\
\n\
Example:\n\
geogen ../examples/atoll.nut out.bmp 2048 2048\n\
\n\
Have a nice day!\n";
		return 0;
	}

	cout << "Initializing...\n";

	if(argc >= 2){
		path_in = argv[1];
	}
	else{
		path_in = new char[12];
		strcpy(path_in, "example.nut");
	}

	if(argc >= 3){
		path_out = argv[2];
	}
	else{
		path_out = "out.bmp";
	}

	/*if(argc >= 4){
		width = atoi(argv[3]);

		if(width < 2){
			cout << "Invalid width!\n";
			return -1;
		}
	}
	else{
		width = 1024;
	}

	if(argc >= 5){
		height = atoi(argv[4]);

		if(height < 2){
			cout << "Invalid height!\n";
			return -1;
		}
	}
	else{
		height = 1024;
	}

	if(argc >= 6){
		seed = atoi(argv[5]);
	}
	else{
		seed = (int) time(0);
	}*/


	//srand((unsigned) seed);
	srand(234);

	// Load the script from file
	string str,strTotal;
	ifstream in;
	in.open(path_in);

	if(!in.is_open()){
		cout << "Could not open the script file!\n";
		return -1;
	}

	getline(in,str);
	while ( in ) {
	   strTotal += "\n" + str;
	   getline(in,str);
	}

	in.close();

	GGen_Squirrel* ggen = new GGen_Squirrel();

	cout << "Compiling...\n";

	ggen->SetReturnCallback(ReturnHandler);

	if(!ggen->SetScript(strTotal.c_str())){
		cout << "Compilation failed!\n";
		delete ggen;
		return -1;
	}

	cout << "Loading map info...\n";

	/*
	if(width > ggen->GetInfoInt("max_width")){
		cout << "Passed width exceeds map's max width!\n";
		delete ggen;
		return -1;
	}

	if(height > ggen->GetInfoInt("max_height")){
		cout << "Passed height exceeds map's max height!\ns"; 
		delete ggen;
		return -1;
	}
	*/

	ggen->LoadArgs();

	// manual mode
	if(argv[3][0] == '?'){
		cout << "	Please set map parameters:\n";
		for(uint8 i = 0; i < ggen->num_args; i++){
			GGen_ScriptArg* a = ggen->args[i];

			char* buf = new char[16];

			cout << "	" << ggen->args[i]->label << " (";
			
			if(ggen->args[i]->type == GGEN_INT) cout << "integer in range " << ggen->args[i]->min_value << "-" << ggen->args[i]->max_value << "): ";
			else if(ggen->args[i]->type == GGEN_BOOL) cout << "0 = No, 1 = Yes): ";
			else if(ggen->args[i]->type == GGEN_ENUM) {
				for(int j = 0; j < ggen->args[i]->num_options; j++){
					if(j > 0) cout << ", ";
					cout << j << " = " << ggen->args[i]->options[j];
				}
				cout << "): ";
			}
			

			cin >> buf;

			// use default value if fist char of the input is not number
			if(buf[0] >= '0' && buf[0] <= '9') ggen->args[i]->SetValue(atoi(buf));
			
			delete buf;
		}		
	}
	// auto mode
	else{
		for(uint8 i = 0; i < ggen->num_args; i++){
			GGen_ScriptArg* a = ggen->args[i];

			if(argc - 3 > i){
				ggen->args[i]->SetValue(atoi(argv[i + 3]));
			}
		}
	}

	//char* name = ggen->GetInfo("description");

/*	if(name == NULL){
		cout << "Map info not found...\n";
		delete ggen;
		return -1;
	}*/

	//cout << name;

	cout << "Executing...\n";

	int16* data = ggen->Generate(123, 132);

	if(data == NULL){
		cout << "Map generation failed!\n";
		delete ggen;
		return -1;		
	}
	
	assert(data != NULL);

	SaveAsBMP(data, ggen->output_width, ggen->output_height, path_out);

	cout << "Cleanup...\n";

	delete ggen;

	cout << "Done!\n";

	return 0;
}