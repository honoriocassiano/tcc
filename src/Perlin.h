#ifndef PERLIN_H_
#define PERLIN_H_

#include "structures/vectors.h"

class Perlin {
public:
	//static Perlin* getInstance();
	static void shuffle();

	static float generate(const Vec3f& position);

	static float generate(float x, float y = 0.0, float z = 0.0);

	static float generateTurbulence(int octaves, int A, int B, const Vec3f& position);

	static float generateTurbulence(int octaves, int A, int B, float x, float y = 0.0, float z = 0.0);

private:
	static int permutation[256];
//	static Perlin* instance;

	static int perm[512];

	static bool shuffled;

//	Perlin();

	static float fade(float t);
	static float lerp(float t, float a, float b);
	static float grad(int hash, float x, float y, float z);
};


#endif
