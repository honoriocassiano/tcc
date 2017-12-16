/*
 * Constants.cpp
 *
 *  Created on: 9 de abr de 2017
 *      Author: cassiano
 */

#include "Constants.h"

namespace pssg {

namespace constant {

unsigned int Shuffle::seed = 0;
std::mt19937 Shuffle::mtRand = std::mt19937();

unsigned int Shuffle::getSeed() {

	if (!seed) {
		init();
	}

	return seed;
}

void Shuffle::init() {
	seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	mtRand = std::mt19937(seed);

	srand(seed);
}

std::mt19937 Shuffle::getRandom() {

	if (!seed) {
		init();
	}

	return mtRand;
}
}

}
