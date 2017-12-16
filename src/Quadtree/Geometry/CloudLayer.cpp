#include "CloudLayer.h"
#include "Planet.h"
#include "../Math/PerlinNoise.h"
#include <algorithm>
#include <stdio.h>

#define TEXTURE_SIZE    2048
#define ANIMATION_SPEED 0.1

using std::max;
using std::min;

unsigned int CloudLayer::instanceCount = 0;
SimpleSphere* CloudLayer::sphere = 0;

CloudLayer::CloudLayer(const Planet *planet, const double radius, const double cloudCover, const double cloudSharpness)
  : planet(planet), radius(radius), texture(0)
{
  // If first instance to be created
  if (!instanceCount++)
    sphere = new SimpleSphere(1.0, 128, 64);

  // Generate texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Generate and use FBO
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  // Store render state and switch to orthographic projection
  glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Use the shader program
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, PerlinNoise<double>::GetPermutationTexture());
  ShaderManager::UseShaderProgram(CLOUD_TEXTURE);
  static const GLint uniformPermTex = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUD_TEXTURE), "permTex");
  glUniform1i(uniformPermTex, 0);
  static const GLint uniformOctaves = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUD_TEXTURE), "octaves");
  glUniform1i(uniformOctaves, 8); // > 0
  static const GLint uniformCloudCover = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUD_TEXTURE), "cloudCover");
  glUniform1f(uniformCloudCover, min(1.0f, max(0.0f, 1.0f - (float)cloudCover)));
  static const GLint uniformCloudSharpness = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUD_TEXTURE), "cloudSharpness");
  glUniform1f(uniformCloudSharpness, (float)cloudSharpness);
  static const GLint uniformFreq0 = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUD_TEXTURE), "freq0");
  glUniform1f(uniformFreq0, 8.0f);

  // Render a square
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2i(-1, -1);
  glVertex2i( 1, -1);
  glVertex2i(-1,  1);
  glVertex2i( 1,  1);
  glEnd();

  // Go back to perspective projection and restore render state
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    printf("FBO ERROR: %d\n", status);

  // Unbind the FBO; return to default state
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteFramebuffers(1, &fbo);
}

CloudLayer::~CloudLayer()
{
  // If last instance to be deleted
  if (!--instanceCount)
    delete sphere;
  glDeleteTextures(1, &texture);
}

double CloudLayer::GetRadius() const
{
  return radius;
}

void CloudLayer::Render() const
{
  // Prepare to render
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glDisable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // These texture weights are used when combining the different cloud textures (w.x + w.y + w.z = 1.0)
  const double time                    = planet->GetAnimationTimeElapsed() * ANIMATION_SPEED;
  const Vector3<double> textureWeights = (Vector3<double>(cos(time), cos(time - 2.0 * M_PI / 3.0), cos(time - 4.0 * M_PI / 3.0))
                                          * 0.5 + 0.5) * 2.0 / 3.0;
  const Vector3<double> lightDirection = planet->GetLightDirection();

  // Use shader program
  ShaderManager::UseShaderProgram(CLOUDS);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  static const GLint uniformLightDir = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUDS), "lightDir");
  glUniform3f(uniformLightDir, (float)lightDirection.x, (float)lightDirection.y, (float)lightDirection.z);
  static const GLint uniformCloudLayerRadius = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUDS), "cloudLayerRadius");
  glUniform1f(uniformCloudLayerRadius, radius);
  static const GLint uniformCloudTex = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUDS), "cloudTex");
  glUniform1i(uniformCloudTex, 0);
  static const GLint uniformCloudTexWeights = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(CLOUDS), "cloudTexWeights");
  glUniform3f(uniformCloudTexWeights, (float)textureWeights.x, (float)textureWeights.y, (float)textureWeights.z);

  // Render the clouds (first inside, then outside)
  glCullFace(GL_FRONT);
  sphere->Render();
  glCullFace(GL_BACK);
  sphere->Render();

  // Restore
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glPopAttrib();
}
