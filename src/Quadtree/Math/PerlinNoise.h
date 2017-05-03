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

#define FLOOR(x)            (((x) >= 0) ? ((int)(x)) : ((int)(x) - 1))
#define DOT(v, x, y, z)     ((v)[0] * (x) + (v)[1] * (y) + (v)[2] * (z))
#define FADE(t)             ((t) * (t) * (t) * ((t) * ((t) * 6 - 15) + 10))
#define FADE_DERIVATIVE(t)  (30 * (t) * (t) * ((t) * ((t) - 2) + 1))
#define LERP(t,a,b)         ((a) + (t) * ((b) - (a)))

template<typename Real>
class PerlinNoise
{
  private:
    static const int  permutationTable[512];
    static const int  gradients[16][3];
    static void       GetIntegerAndFractionalParts(const Real x, int &x0i, int &x1i, Real &x0f, Real &x1f);
    static GLuint     permutationTexture;

  public:
    static Real   GetValue(const Real x, const Real y = Real(0.0), const Real z = Real(0.0));
    static GLuint GetPermutationTexture();
};

template<typename Real>
const int PerlinNoise<Real>::permutationTable[512] = { 151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
                                                       140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
                                                       247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
                                                        57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
                                                        74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
                                                        60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
                                                        65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
                                                       200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
                                                        52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
                                                       207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
                                                       119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
                                                       129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
                                                       218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
                                                        81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
                                                       184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
                                                       222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
                                                       151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
                                                       140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
                                                       247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
                                                        57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
                                                        74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
                                                        60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
                                                        65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
                                                       200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
                                                        52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
                                                       207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
                                                       119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
                                                       129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
                                                       218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
                                                        81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
                                                       184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
                                                       222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180 };

template<typename Real>
const int PerlinNoise<Real>::gradients[16][3] = { {  1,  1,  0 }, { -1,  1,  0 }, {  1, -1,  0 }, { -1, -1,  0 },
                                                  {  1,  0,  1 }, { -1,  0,  1 }, {  1,  0, -1 }, { -1,  0, -1 },
                                                  {  0,  1,  1 }, {  0, -1,  1 }, {  0,  1, -1 }, {  0, -1, -1 },
                                                  {  1,  1,  0 }, {  0, -1,  1 }, { -1,  1,  0 }, {  0, -1, -1 } };

template<typename Real>
GLuint PerlinNoise<Real>::permutationTexture = 0;

template<typename Real>
void PerlinNoise<Real>::GetIntegerAndFractionalParts(const Real x, int &x0i, int &x1i, Real &x0f, Real &x1f)
{
  x0i = FLOOR(x);
  x0f = x - x0i;
  x1f = x0f - Real(1.0);
  x1i = (x0i + 1) & 255;
  x0i &= 255;
}

template<typename Real>
Real PerlinNoise<Real>::GetValue(const Real x, const Real y, const Real z)
{
  int x0i, x1i, y0i, y1i, z0i, z1i;
  Real x0f, x1f, y0f, y1f, z0f, z1f;
  GetIntegerAndFractionalParts(x, x0i, x1i, x0f, x1f);
  GetIntegerAndFractionalParts(y, y0i, y1i, y0f, y1f);
  GetIntegerAndFractionalParts(z, z0i, z1i, z0f, z1f);

  const Real u = FADE(x0f);
  const Real v = FADE(y0f);
  const Real w = FADE(z0f);

  const Real a = DOT(gradients[permutationTable[x0i + permutationTable[y0i + permutationTable[z0i]]] & 15], x0f, y0f, z0f);
  const Real b = DOT(gradients[permutationTable[x1i + permutationTable[y0i + permutationTable[z0i]]] & 15], x1f, y0f, z0f);
  const Real c = DOT(gradients[permutationTable[x0i + permutationTable[y1i + permutationTable[z0i]]] & 15], x0f, y1f, z0f);
  const Real d = DOT(gradients[permutationTable[x1i + permutationTable[y1i + permutationTable[z0i]]] & 15], x1f, y1f, z0f);
  const Real e = DOT(gradients[permutationTable[x0i + permutationTable[y0i + permutationTable[z1i]]] & 15], x0f, y0f, z1f);
  const Real f = DOT(gradients[permutationTable[x1i + permutationTable[y0i + permutationTable[z1i]]] & 15], x1f, y0f, z1f);
  const Real g = DOT(gradients[permutationTable[x0i + permutationTable[y1i + permutationTable[z1i]]] & 15], x0f, y1f, z1f);
  const Real h = DOT(gradients[permutationTable[x1i + permutationTable[y1i + permutationTable[z1i]]] & 15], x1f, y1f, z1f);

  return LERP(u, LERP(v, LERP(w, a, e), LERP(w, c, g)), LERP(v, LERP(w, b, f), LERP(w, d, h)));
}

template<typename Real>
GLuint PerlinNoise<Real>::GetPermutationTexture()
{
  if (!permutationTexture)
  {
    GLubyte pixels[256 * 256 * 4];
    unsigned int index = 0;

    for (unsigned int y = 0; y < 256; y++)
      for (unsigned int x = 0; x < 256; x++)
      {
        const GLubyte value     = permutationTable[x + permutationTable[y]];
        const int gradientIndex = permutationTable[x + y] & 15;
        pixels[index++]         = (1 + gradients[gradientIndex][2]) * 64;
        pixels[index++]         = (1 + gradients[gradientIndex][1]) * 64;
        pixels[index++]         = (1 + gradients[gradientIndex][0]) * 64;
        pixels[index++]         = value;
      }

    glGenTextures(1, &permutationTexture);
    glBindTexture(GL_TEXTURE_2D, permutationTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
  }

  return permutationTexture;
}

#endif
