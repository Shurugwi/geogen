/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
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

#include "ggen.h"
#include "ggen_squirrel.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"

extern GGen* ggen_current_object;

GGen_Amplitudes::GGen_Amplitudes(uint16 max_feature_size){ 
	uint8 size = GGen_log2(max_feature_size);
	
	data = new uint16[size];

	GGen_Script_Assert(data != NULL);

	for(uint8 i = 0; i < size; i++){
		data[i] = 0;
	}

};

void GGen_Amplitudes::AddAmplitude(uint16 feature_size, uint16 amplitude){
	data[GGen_log2(feature_size)] = amplitude;
}