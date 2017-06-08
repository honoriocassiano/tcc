#include "Randomizer.h"

#define RANDOM_MAX  0x7FFF
#define LCG_A       214013
#define LCG_C       2531011

int Randomizer::state = 0;
std::mt19937 Randomizer::engine = std::mt19937(state);

// Seeds the randomizer
void Randomizer::Seed(const int seed) {
	state = seed;

	engine = std::mt19937(seed);
}

// Returns a pseudorandom integer [0,RANDOM_MAX]
int Randomizer::GetInteger() {
	state = LCG_A * state + LCG_C;
	return (state >> 16) & RANDOM_MAX;
}

// Returns a pseudorandom integer [min, max]
int Randomizer::GetInteger(const int min, const int max) {
	return min + GetInteger() % (max - min);
}

// Returns a pseudorandom float [0,1]
float Randomizer::GetFloat() {
	return GetInteger() / (float) RANDOM_MAX;
}

// Returns a pseudorandom float [min, max]
float Randomizer::GetFloat(const float min, const float max) {
	return min + (max - min) * GetFloat();
}

// Returns pseudorandom spherical coordinates (radius can be obtained as a separate float value)
void Randomizer::GetSphericalCoordinates(float &theta, float &phi) {
	const float u = GetFloat();
	const float v = GetFloat();
	theta = 2.0 * M_PI * u;
	phi = acos(2.0 * v - 1.0);
}

// Returns a pseudorandom 3D direction
void Randomizer::GetDirection(float &x, float &y, float &z) {
	float theta, phi;
	GetSphericalCoordinates(theta, phi);
	x = cos(theta) * sin(phi);
	y = sin(theta) * sin(phi);
	z = cos(phi);
}

std::mt19937 Randomizer::getEngine() {
	return engine;
}
