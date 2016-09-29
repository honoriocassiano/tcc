#ifndef PERLIN_H_
#define PERLIN_H_

#include "structures/vectors.h"

/**
 * Perlin noise functions.
 */
class Perlin {
public:

	/**
	 * Generate noise by vector position.
	 */
	static float generate(const Vec3f& position);

	/**
	 * Generate noise by parameter position.
	 */
	static float generate(float x, float y = 0.0, float z = 0.0);

	/**
	 * Generate turbulence by vector position.
	 * @param octaves Octaves to be used
	 * @param A Roughness of noise
	 * @param B %Scale of noise
	 * @param position Noise position
	 *
	 * @return Value between -1 and 1
	 */
	static float generateTurbulence(int octaves, int A, int B,
			const Vec3f& position);

	/**
	 * Generate turbulence by parameters position.
	 * @see generateTurbulence(int, int, int, const Vec3f&)
	 */
	static float generateTurbulence(int octaves, int A, int B, float x,
			float y = 0.0, float z = 0.0);

private:

	/**
	 * Shuffle permutation vector.
	 */
	static void shuffle();

	static int permutation[256];

	static int perm[512];


	/**
	 * Flag that indicate if permutation vector was shuffled.
	 */
	static bool shuffled;

	/**
	 * Perlin noise interpolation function
	 */
	static float fade(float t);

	/**
	 * Linear interpolation between two values.
	 *
	 * @param a First value
	 * @param b Second value
	 * @param t Interpolation factor
	 *
	 * @return Interpolated value.
	 */
	static float lerp(float t, float a, float b);
	static float grad(int hash, float x, float y, float z);
};

#endif
