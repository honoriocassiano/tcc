#include "Skybox.h"
#include "../Math/VectorMath.h"
#include "../Shaders/ShaderManager.h"
#include "../Math/PerlinNoise.h"

#define TEXTURE_SIZE 512

struct SkyboxVertex
{
  Vector3<GLfloat> position;
  Vector2<GLfloat> texCoord;
  SkyboxVertex(const float x, const float y, const float z, const float s, const float t) : position(x, y, z), texCoord(s, t) {}
};

struct SkyboxSide
{
  SkyboxVertex v0, v1, v2, v3;
  SkyboxSide(const SkyboxVertex &v0, const SkyboxVertex &v1, const SkyboxVertex &v2, const SkyboxVertex &v3) : v0(v0), v1(v1), v2(v2), v3(v3) {}
};

Skybox::Skybox()
  : textures(), displayList(0)
{
  // Cube sides in default OpenGL cube map order
  static SkyboxSide sides[SKYBOX_SIDES] = { SkyboxSide(SkyboxVertex( 1, -1,  1,  0,  0), SkyboxVertex( 1, -1, -1,  1,  0), SkyboxVertex( 1,  1, -1,  1,  1), SkyboxVertex( 1,  1,  1,  0,  1)),
                                            SkyboxSide(SkyboxVertex(-1, -1, -1,  0,  0), SkyboxVertex(-1, -1,  1,  1,  0), SkyboxVertex(-1,  1,  1,  1,  1), SkyboxVertex(-1,  1, -1,  0,  1)),
                                            SkyboxSide(SkyboxVertex(-1,  1,  1,  0,  0), SkyboxVertex( 1,  1,  1,  1,  0), SkyboxVertex( 1,  1, -1,  1,  1), SkyboxVertex(-1,  1, -1,  0,  1)),
                                            SkyboxSide(SkyboxVertex( 1, -1,  1,  0,  0), SkyboxVertex(-1, -1,  1,  1,  0), SkyboxVertex(-1, -1, -1,  1,  1), SkyboxVertex( 1, -1, -1,  0,  1)),
                                            SkyboxSide(SkyboxVertex(-1, -1,  1,  0,  0), SkyboxVertex( 1, -1,  1,  1,  0), SkyboxVertex( 1,  1,  1,  1,  1), SkyboxVertex(-1,  1,  1,  0,  1)),
                                            SkyboxSide(SkyboxVertex( 1, -1, -1,  0,  0), SkyboxVertex(-1, -1, -1,  1,  0), SkyboxVertex(-1,  1, -1,  1,  1), SkyboxVertex( 1,  1, -1,  0,  1)) };

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

  // Generate and use FBO
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Create side textures
  for (int i = 0; i < SKYBOX_SIDES; i++)
  {
    // Generate texture
    glGenTextures(1, &textures[i]);
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);

    // Use the shader program
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, PerlinNoise<double>::GetPermutationTexture());
    ShaderManager::UseShaderProgram(SKYBOX_TEXTURE);
    static const GLint uniformPermTex = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(SKYBOX_TEXTURE), "permTex");
    glUniform1i(uniformPermTex, 0);

    // Render a square (using cube positions as texture coordinates)
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord3fv(sides[i].v0.position.data);
    glVertex2i(-1, -1);
    glTexCoord3fv(sides[i].v1.position.data);
    glVertex2i( 1, -1);
    glTexCoord3fv(sides[i].v3.position.data);
    glVertex2i(-1,  1);
    glTexCoord3fv(sides[i].v2.position.data);
    glVertex2i( 1,  1);
    glEnd();
  }

  // Go back to perspective projection and restore render state
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  // Unbind the FBO; return to default state
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteFramebuffers(1, &fbo);

  // Generate display list
  displayList = glGenLists(1);
  glNewList(displayList, GL_COMPILE);
  for (int i = 0; i < SKYBOX_SIDES; i++)
  {
    glBindTexture(GL_TEXTURE_2D, textures[i]);

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(sides[i].v0.texCoord.data);
    glVertex3fv(sides[i].v0.position.data);
    glTexCoord2fv(sides[i].v1.texCoord.data);
    glVertex3fv(sides[i].v1.position.data);
    glTexCoord2fv(sides[i].v3.texCoord.data);
    glVertex3fv(sides[i].v3.position.data);
    glTexCoord2fv(sides[i].v2.texCoord.data);
    glVertex3fv(sides[i].v2.position.data);
    glEnd();
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glEndList();
}

Skybox::~Skybox()
{
  glDeleteLists(displayList, 1);
}

void Skybox::Render() const
{
  if (!displayList)
    return;

  // Remove translation part from modelview matrix
  glPushMatrix();
  GLdouble modelview[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  modelview[12] = modelview[13] = modelview[14] = 0.0;
  glLoadMatrixd(modelview);

  // Prepare to render
  glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glDepthMask(GL_FALSE);
  glEnable(GL_TEXTURE_2D);

  // Render
  glCallList(displayList);

  // Restore render state
  glPopAttrib();
  glPopMatrix();
}
