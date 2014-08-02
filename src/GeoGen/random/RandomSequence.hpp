#pragma once

#include "RandomGenerator.hpp"
#include "../Number.hpp"
#include "mtrand/MTRand.hpp"

namespace geogen
{
	namespace random
	{
		class RandomSequence
		{
		private:
			mtrand::MTRand_int32 mtRand;
		public:
			RandomSequence(RandomSeed seed);

			inline int NextInt() { return mtRand(); }
			inline int NextInt(int min, int max) { return min + (this->NextInt() % (int)(max - min + 1)); }
			inline Height NextHeight() { return (Height)NextInt(HEIGHT_MIN, HEIGHT_MAX); }
			inline double NextDouble() { return (double)(NextInt()) * (1. / 4294967295.); }
		};
	}
}