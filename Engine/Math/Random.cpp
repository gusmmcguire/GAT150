#include "Random.h"
#include <stdlib.h>

namespace gme {
	void SeedRandom(unsigned int seed){
		srand(seed);
	}
	float Random() {
		return rand() / static_cast<float>(RAND_MAX);
	}

	float RandomRange(float min, float max) {
		return min + (max - min) * Random();
	}
	int RandomInt(){
		return rand();
	}
	int RandomRangeInt(int min, int max){
		return min + rand() % (max - min);
	}
}