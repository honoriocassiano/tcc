#ifndef PERLIN_H_
#define PERLIN_H_

class Perlin {
public:
	static Perlin* getInstance();

	float generate(float x, float y = 0.0, float z = 0.0);

private:
	static int permutation[256];
	static Perlin* instance;

	int perm[512];

	Perlin();

	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
};


#endif
