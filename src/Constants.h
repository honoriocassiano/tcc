/*
 * Constants.h
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include "Scale.h"

#include <random>
#include <chrono>

/**
 * Define some constants used by program.
 */
namespace constant {

/**
 * Get Gravitational Constant value
 *
 * Return the value of Gravitational Constant (6.67408 × 10<SUP>-11</SUP> m<SUP>3</SUP> kg<SUP>-1</SUP> s<SUP>-2</SUP>) relative to length scale.
 */
inline double G() {
	return 6.67e-11 * Scale::getLength();
}

class Shuffle {
public:
	static unsigned int getSeed();

	static std::mt19937 getRandom();

private:

	static void init();

	static unsigned int seed;
	static std::mt19937 mtRand;
};

}

#endif /* SRC_CONSTANTS_H_ */
