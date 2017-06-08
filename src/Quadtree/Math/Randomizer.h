/*******************************************************************************
 * File:           Randomizer.h
 * Author:         Christian Alfons
 * Date created:   2011-01-23
 * Date modified:  2011-01-24
 * Description:    Pseudorandom number generator class with simple but useful
 *                 methods. The linear congruential generator (LCG) ensures
 *                 consistent cross-platform results.
 *******************************************************************************/

#ifndef RANDOMIZER_H_INCLUDED
#define RANDOMIZER_H_INCLUDED

#include <cmath>
#include <random>

#define RANDOM_MAX  0x7FFF
#define LCG_A       214013
#define LCG_C       2531011

class Randomizer {
private:
	// Class variables
	static int state;

	static std::mt19937 engine;

public:
	// Class methods
	static void Seed(const int seed);
	static int GetInteger();
	static int GetInteger(const int min, const int max);
	static float GetFloat();
	static float GetFloat(const float min, const float max);
	static void GetSphericalCoordinates(float &theta, float &phi);
	static void GetDirection(float &x, float &y, float &z);

	static std::mt19937 getEngine();
};

#endif
