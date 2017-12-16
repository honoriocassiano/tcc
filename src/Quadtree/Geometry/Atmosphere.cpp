#include "Atmosphere.h"
#include "Planet.h"
#include "../Shaders/ShaderManager.h"
#include <algorithm>

using std::max;
using std::min;

unsigned int Atmosphere::instanceCount = 0;
SimpleSphere* Atmosphere::sphere = 0;

Atmosphere::Atmosphere(const Planet *planet, const double radius, const Vector3<float> &color)
  : planet(planet), radius(radius), color(color)
{
  // If first instance to be created
  if (!instanceCount++)
    sphere = new SimpleSphere(1.0, 128, 64);
}

Atmosphere::~Atmosphere()
{
  // If last instance to be deleted
  if (!--instanceCount)
    delete sphere;
}

Vector3<float> Atmosphere::GetColor() const
{
  return color;
}

double Atmosphere::GetRadius() const
{
  return radius;
}

void Atmosphere::Render() const
{
  if (!planet)
    return;

  // Prepare to render
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glDisable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // Use shader program
  ShaderManager::UseShaderProgram(ATMOSPHERE);
  static const GLint uniformLightDir = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "lightDir");
  glUniform3f(uniformLightDir, (float)planet->GetLightDirection().x, (float)planet->GetLightDirection().y, (float)planet->GetLightDirection().z);
  static const GLint uniformPlanetRadius = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "planetRadius");
  glUniform1f(uniformPlanetRadius, planet->GetTerrain()->GetRadius());
  static const GLint uniformAtmosphereRadius = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "atmosphereRadius");
  glUniform1f(uniformAtmosphereRadius, radius);
  static const GLint uniformCamPos = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "camPos");
  glUniform3f(uniformCamPos, (float)planet->GetTerrain()->GetCameraPositionLocal().x, (float)planet->GetTerrain()->GetCameraPositionLocal().y, (float)planet->GetTerrain()->GetCameraPositionLocal().z);
  static const GLint uniformCamHeight = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "camHeight");
  glUniform1f(uniformCamHeight, planet->GetTerrain()->GetCameraPositionLocal().GetLength());
  static const GLint uniformAtmosphereColor = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(ATMOSPHERE), "atmosphereColor");
  glUniform3fv(uniformAtmosphereColor, 1, color.data);

  // Render
  glCullFace(GL_FRONT);
  sphere->Render();

  // Restore
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glPopAttrib();
}
