/*******************************************************************************
 * File:           PerlinNoise.h
 * Author:         Christian Alfons
 * Date created:   2010-10-26
 * Date modified:  2010-11-07
 * Description:    Improved Perlin noise template class based on Ken Perlin's
 *                 reference implementation.
 *******************************************************************************/

#ifndef PERLINNOISE_H_INCLUDED
#define PERLINNOISE_H_INCLUDED

#include <GL/gl.h>
#include "Randomizer.h"

#define FLOOR(x)            (((x) >= 0) ? ((int)(x)) : ((int)(x) - 1))
#define DOT(v, x, y, z)     ((v)[0] * (x) + (v)[1] * (y) + (v)[2] * (z))
#define FADE(t)             ((t) * (t) * (t) * ((t) * ((t) * 6 - 15) + 10))
#define FADE_DERIVATIVE(t)  (30 * (t) * (t) * ((t) * ((t) - 2) + 1))
#define LERP(t,a,b)         ((a) + (t) * ((b) - (a)))

template<typename Real>
class PerlinNoise {
private:
	static int permutationTable[512];
	static const int gradients[16][3];
	static void GetIntegerAndFractionalParts(const Real x, int &x0i, int &x1i,
			Real &x0f, Real &x1f);
	static GLuint permutationTexture;

public:
	static Real GetValue(const Real x, const Real y = Real(0.0), const Real z =
			Real(0.0));
	static GLuint GetPermutationTexture();
};

template<typename Real>
int PerlinNoise<Real>::permutationTable[512] = {};

template<typename Real>
const int PerlinNoise<Real>::gradients[16][3] = { { 1, 1, 0 }, { -1, 1, 0 }, {
		1, -1, 0 }, { -1, -1, 0 }, { 1, 0, 1 }, { -1, 0, 1 }, { 1, 0, -1 }, {
		-1, 0, -1 }, { 0, 1, 1 }, { 0, -1, 1 }, { 0, 1, -1 }, { 0, -1, -1 }, {
		1, 1, 0 }, { 0, -1, 1 }, { -1, 1, 0 }, { 0, -1, -1 } };

template<typename Real>
GLuint PerlinNoise<Real>::permutationTexture = 0;

template<typename Real>
void PerlinNoise<Real>::GetIntegerAndFractionalParts(const Real x, int &x0i,
		int &x1i, Real &x0f, Real &x1f) {
	x0i = FLOOR(x);
	x0f = x - x0i;
	x1f = x0f - Real(1.0);
	x1i = (x0i + 1) & 255;
	x0i &= 255;
}

template<typename Real>
Real PerlinNoise<Real>::GetValue(const Real x, const Real y, const Real z) {
	int x0i, x1i, y0i, y1i, z0i, z1i;
	Real x0f, x1f, y0f, y1f, z0f, z1f;
	GetIntegerAndFractionalParts(x, x0i, x1i, x0f, x1f);
	GetIntegerAndFractionalParts(y, y0i, y1i, y0f, y1f);
	GetIntegerAndFractionalParts(z, z0i, z1i, z0f, z1f);

	const Real u = FADE(x0f);
	const Real v = FADE(y0f);
	const Real w = FADE(z0f);

	const Real a = DOT(
			gradients[permutationTable[x0i
					+ permutationTable[y0i + permutationTable[z0i]]] & 15], x0f,
			y0f, z0f);
	const Real b = DOT(
			gradients[permutationTable[x1i
					+ permutationTable[y0i + permutationTable[z0i]]] & 15], x1f,
			y0f, z0f);
	const Real c = DOT(
			gradients[permutationTable[x0i
					+ permutationTable[y1i + permutationTable[z0i]]] & 15], x0f,
			y1f, z0f);
	const Real d = DOT(
			gradients[permutationTable[x1i
					+ permutationTable[y1i + permutationTable[z0i]]] & 15], x1f,
			y1f, z0f);
	const Real e = DOT(
			gradients[permutationTable[x0i
					+ permutationTable[y0i + permutationTable[z1i]]] & 15], x0f,
			y0f, z1f);
	const Real f = DOT(
			gradients[permutationTable[x1i
					+ permutationTable[y0i + permutationTable[z1i]]] & 15], x1f,
			y0f, z1f);
	const Real g = DOT(
			gradients[permutationTable[x0i
					+ permutationTable[y1i + permutationTable[z1i]]] & 15], x0f,
			y1f, z1f);
	const Real h = DOT(
			gradients[permutationTable[x1i
					+ permutationTable[y1i + permutationTable[z1i]]] & 15], x1f,
			y1f, z1f);

	return LERP(u, LERP(v, LERP(w, a, e), LERP(w, c, g)),
			LERP(v, LERP(w, b, f), LERP(w, d, h)));
}

template<typename Real>
GLuint PerlinNoise<Real>::GetPermutationTexture() {
	if (!permutationTexture) {

		int temp[256];

		for (unsigned int i = 0; i < 256; i++) {
			temp[i] = i + 1;
		}

		std::shuffle(&temp[0], &temp[256],
				Randomizer::getEngine());

		for (unsigned int i = 0; i < 256; i++) {
			permutationTable[i] = temp[i];
			permutationTable[i + 256] = temp[i];
		}

		GLubyte pixels[256 * 256 * 4];
		unsigned int index = 0;

		for (unsigned int y = 0; y < 256; y++)
			for (unsigned int x = 0; x < 256; x++) {
				const GLubyte value = permutationTable[x + permutationTable[y]];
				const int gradientIndex = permutationTable[x + y] & 15;
				pixels[index++] = (1 + gradients[gradientIndex][2]) * 64;
				pixels[index++] = (1 + gradients[gradientIndex][1]) * 64;
				pixels[index++] = (1 + gradients[gradientIndex][0]) * 64;
				pixels[index++] = value;
			}

		glGenTextures(1, &permutationTexture);
		glBindTexture(GL_TEXTURE_2D, permutationTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, pixels);
	}

	return permutationTexture;
}

#endif
