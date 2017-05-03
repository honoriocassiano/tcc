#include "Starfield.h"
#include "../Math/Randomizer.h"
#include "../Math/VectorMath.h"

#define BUFFER_OFFSET(i) ((char*)0 + (i))

unsigned int Starfield::instanceCount = 0;
GLuint Starfield::texture = 0;

struct Vertex
{
  Vector3<GLfloat> position;
  Vector2<GLfloat> texCoord;
  Vertex() {}
  Vertex(const Vector3<GLfloat> &position, const Vector2<GLfloat> texCoord) : position(position), texCoord(texCoord) {}
  Vertex(const float x, const float y, const float z, const float s, const float t) : position(x, y, z), texCoord(s, t) {}
};

struct Billboard
{
  Vertex v0, v1, v2, v3;
  Billboard() {}
  Billboard(const Vertex &v0, const Vertex &v1, const Vertex &v2, const Vertex &v3) : v0(v0), v1(v1), v2(v2), v3(v3) {}
};

Starfield::Starfield(const unsigned int numberOfStars, const float starSizeMin, const float starSizeMax)
  : vbo(0), ibo(0), elementCount(6 * numberOfStars)
{
  // If first instance to be created
  if (!instanceCount++)
    texture = Star::GenerateStarTexture(16);

  Billboard billboards[numberOfStars];
  GLushort inds[elementCount];

  Randomizer::Seed(0);

  // Create stars
  int index = 0;
  for (unsigned int i = 0; i < numberOfStars; i++)
  {
    // Randomize star size
    const float starSize = Randomizer::GetFloat(starSizeMin, starSizeMax);

    // Randomize star position
    float theta, phi;
    Randomizer::GetSphericalCoordinates(theta, phi);
    const Vector3<float> zDir(sin(theta) * sin(phi),  cos(phi), cos(theta) * sin(phi));
    const Vector3<float> yDir(sin(theta) * cos(phi), -sin(phi), cos(theta) * cos(phi));
    const Vector3<float> xDir = yDir.Cross(zDir);

    const float scale = 0.001 * starSize;
    billboards[i] = Billboard(Vertex(zDir - xDir * scale - yDir * scale, Vector2<float>(0.0, 0.0)),
                              Vertex(zDir + xDir * scale - yDir * scale, Vector2<float>(1.0, 0.0)),
                              Vertex(zDir + xDir * scale + yDir * scale, Vector2<float>(1.0, 1.0)),
                              Vertex(zDir - xDir * scale + yDir * scale, Vector2<float>(0.0, 1.0)));

    const int offset = i * 4;
    inds[index++] = offset + 0;
    inds[index++] = offset + 2;
    inds[index++] = offset + 1;
    inds[index++] = offset + 0;
    inds[index++] = offset + 3;
    inds[index++] = offset + 2;
  }

  // Generate and initialize VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Billboard) * numberOfStars, billboards, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate and initialize IBO
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * elementCount, inds, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Starfield::~Starfield()
{
  // If last instance to be deleted
  if (!--instanceCount)
  {
    glDeleteTextures(1, &texture);
    texture = 0;
  }

  glDeleteBuffers(1, &ibo);
  glDeleteBuffers(1, &vbo);
}

void Starfield::Render() const
{
  // Remove translation part from modelview matrix
  glPushMatrix();
  GLdouble modelview[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  modelview[12] = modelview[13] = modelview[14] = 0.0;
  glLoadMatrixd(modelview);

  // Prepare to render
  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
  glDepthMask(GL_FALSE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // Bind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  //glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glTexCoordPointer(2,  GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(Vector3<GLfloat>)));
  //glNormalPointer(      GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(Vector3<GLfloat>)));
  glVertexPointer(  3,  GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));

  // Render
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  ShaderManager::UseShaderProgram(STAR);
  static const GLint uniformStarTex = glGetUniformLocation(ShaderManager::GetShaderProgramHandle(STAR), "starTex");
  glUniform1i(uniformStarTex, 0);
  glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);

  // Unbind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  //glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // Restore render state
  glPopAttrib();
  glPopMatrix();
}
