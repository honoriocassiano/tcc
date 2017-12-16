#include "SimpleSphere.h"
#include <cmath>

#define BUFFER_OFFSET(i) ((char*)0 + (i))
#define VERT_NUM_FLOATS 8

SimpleSphere::SimpleSphere(const float radius, const unsigned int longSegs, const unsigned int latSegs)
  : vbo(0), ibo(0), elementCount(0)
{
  // Calculate vertex positions
  const unsigned int longVerts = longSegs + 1, latVerts = latSegs + 1, vertCount = longVerts * latVerts;
  GLfloat verts[longVerts][latVerts][VERT_NUM_FLOATS];
  for (unsigned int i = 0; i < longVerts; i++)
  {
    const float iDivLong  = i / (float)longSegs;
    const float theta     = i == 0 || i == longSegs ? 0.0 : iDivLong * 2.0 * M_PI;
    for (unsigned int j = 0; j < latVerts; j++)
    {
      const float jDivLat = j / (float)latSegs;
      const float phi     = jDivLat * M_PI;

      // Normal
      verts[i][j][3]  = cos(theta) * sin(phi);
      verts[i][j][4]  = cos(phi);
      verts[i][j][5]  = sin(theta) * sin(phi);

      // Position
      verts[i][j][0]  = verts[i][j][3] * radius;
      verts[i][j][1]  = verts[i][j][4] * radius;
      verts[i][j][2]  = verts[i][j][5] * radius;

      // Texture coordinates
      verts[i][j][6]  = iDivLong;
      verts[i][j][7]  = jDivLat;
    }
  }

  // Build triangles
  const unsigned int triCount = longSegs * latSegs * 2;
  elementCount = triCount * 3;
  GLushort tris[elementCount];
  unsigned int index = 0;
  for (unsigned int i = 0; i < longSegs; i++)
    for (unsigned int j = 0; j < latSegs; j++)
    {
      // Vertex indices
      const unsigned int v0 = j + latVerts * i;
      const unsigned int v1 = j + latVerts * (i + 1);
      const unsigned int v2 = v1 + 1;
      const unsigned int v3 = v0 + 1;

      // First triangle
      tris[index++]         = v0;
      tris[index++]         = v1;
      tris[index++]         = v2;

      // Second triangle
      tris[index++]         = v0;
      tris[index++]         = v2;
      tris[index++]         = v3;
    }

  // Generate and initialize VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERT_NUM_FLOATS * vertCount, verts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate and initialize IBO
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * elementCount, tris, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SimpleSphere::~SimpleSphere()
{
  glDeleteBuffers(1, &ibo);
  glDeleteBuffers(1, &vbo);
}

void SimpleSphere::Render() const
{
  // Bind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glTexCoordPointer(2,  GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(sizeof(GLfloat) * 3 * 2));
  glNormalPointer(      GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(sizeof(GLfloat) * 3));
  glVertexPointer(  3,  GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(0));

  // Render
  glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  // Unbind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
