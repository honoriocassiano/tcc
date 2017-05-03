#include "SimpleDisk.h"
#include <cmath>

#define BUFFER_OFFSET(i) ((char*)0 + (i))
#define VERT_NUM_FLOATS 4

SimpleDisk::SimpleDisk(const float innerRadius, const float outerRadius, const unsigned int slices, const unsigned int loops)
  : vbo(0), ibo(0), elementCount(0)
{
  // Calculate vertex positions
  const unsigned int vertCount = slices * (loops + 1);
  GLfloat verts[vertCount * VERT_NUM_FLOATS];
  unsigned int index = 0;
  for (unsigned int i = 0; i < slices; i++)
  {
    const float iDivSlices  = i / (float)slices;
    const float alpha = iDivSlices * 2.0 * M_PI;
    const float xDir = cos(alpha), yDir = sin(alpha);
    for (unsigned int j = 0; j <= loops; j++)
    {
      const float jDivLoops = j / (float)loops;
      const float radius = innerRadius + jDivLoops * (outerRadius - innerRadius);

      // Position
      verts[index++] = xDir * radius;
      verts[index++] = yDir * radius;

      // Texture coordinates
      verts[index++] = iDivSlices;
      verts[index++] = jDivLoops;
    }
  }

  // Build triangles
  const unsigned int triCount = slices * loops * 2;
  elementCount = triCount * 3;
  GLushort tris[elementCount];
  index = 0;
  for (unsigned int i = 0; i < slices; i++)
    for (unsigned int j = 0; j < loops; j++)
    {
      // Vertex indices
      const unsigned int v0 = i * (loops + 1) + j;
      const unsigned int v1 = (v0 + loops + 1) % vertCount;
      const unsigned int v2 = v1 + 1;
      const unsigned int v3 = v0 + 1;

      // First triangle
      tris[index++]         = v0;
      tris[index++]         = v2;
      tris[index++]         = v1;

      // Second triangle
      tris[index++]         = v0;
      tris[index++]         = v3;
      tris[index++]         = v2;
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

SimpleDisk::~SimpleDisk()
{
  glDeleteBuffers(1, &ibo);
  glDeleteBuffers(1, &vbo);
}

void SimpleDisk::Render() const
{
  // Bind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glTexCoordPointer(2,  GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(sizeof(GLfloat) * 2));
  glVertexPointer(  2,  GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(0));

  // Render
  glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  // Unbind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
