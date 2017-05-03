#include "PlanetRing.h"
#include "Planet.h"
#include "../Math/PerlinNoise.h"

#define TEXTURE_SIZE 256
#define RING_OCTAVE0 2
#define RING_OCTAVES 10

PlanetRing::PlanetRing(const Planet *planet, const double innerRadius, const double outerRadius, const Vector4<double> &color)
  : planet(planet), disk(new SimpleDisk(innerRadius, outerRadius, 128, 1))
{
  // Calculate a 1D texture (first texel is closest to the planet; last one is farthest away)
  GLubyte pixels[TEXTURE_SIZE][4];
  for (unsigned int i = 0; i < TEXTURE_SIZE; i++)
  {
    const double r = i / (float)(TEXTURE_SIZE - 1);
    const Vector4<double> pixelColor = color;

    double a = 0.0;
    double f = RING_OCTAVE0;
    double amp = 20.0;
    if (i != 0 && i != TEXTURE_SIZE - 1)
    {
      for (unsigned int j = RING_OCTAVE0; j < RING_OCTAVE0 + RING_OCTAVES; j++)
      {
        a += amp * PerlinNoise<double>::GetValue(4.0 + f * r);
        amp *= 0.5;
        f *= 2.1;
      }
      a /= (double)RING_OCTAVES;
      a = 0.5 + 0.5 * a;
      a = a * a * a;
      //a = fabs(a);
    }
    pixels[i][0] = (int)(255 * pixelColor.b);
    pixels[i][1] = (int)(255 * pixelColor.g);
    pixels[i][2] = (int)(255 * pixelColor.r);
    pixels[i][3] = (int)(255 * color.a* a);
  }

  // Generate texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_1D, texture);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, TEXTURE_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_1D, 0);
}

PlanetRing::~PlanetRing()
{
  delete disk;
  glDeleteTextures(1, &texture);
}

void PlanetRing::Render() const
{
  // Prepare to render
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_1D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Use shader program
  glBindTexture(GL_TEXTURE_1D, texture);
  ShaderManager::UseShaderProgram(PLANET_RING);
  static const GLint uniformLightDir = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(PLANET_RING), "lightDir");
  glUniform3f(uniformLightDir, (float)planet->GetLightDirection().x, (float)planet->GetLightDirection().y, (float)planet->GetLightDirection().z);
  static const GLint uniformRingTex = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(PLANET_RING), "ringTex");
  glUniform1i(uniformRingTex, 0);

  // Render rotated (xz-plane instead of xy-plane)
  glPushMatrix();
  glRotated(90.0, 1.0, 0.0, 0.0);
  disk->Render();
  glPopMatrix();

  // Restore
  glBindTexture(GL_TEXTURE_1D, 0);
  glUseProgram(0);
  glPopAttrib();
}
