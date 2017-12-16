#include "SimpleCylinder.h"
#include <cmath>

#define BUFFER_OFFSET(i) ((char*)0 + (i))
#define VERT_NUM_FLOATS 8

SimpleCylinder::SimpleCylinder(const float radiusBase, const float radiusTop, const float height, const unsigned int slices, const unsigned int stacks, const float y0)
  : vbo(0), ibo(0), elementCount(0)
{
  // Calculate vertex positions
  const unsigned int sliceVerts = slices + 1, stackVerts = stacks + 1, vertCount = sliceVerts * stackVerts;
  GLfloat verts[vertCount * VERT_NUM_FLOATS];
  unsigned int index = 0;
  for (unsigned int i = 0; i < sliceVerts; i++)
  {
    const float iDivSlices  = i / (float)slices;
    const float alpha       = i == 0 || i == slices ? 0.0 : iDivSlices * 2.0 * M_PI;
    const float xDir = cos(alpha), zDir = sin(alpha);
    for (unsigned int j = 0; j < stackVerts; j++)
    {
      const float jDivStacks  = j / (float)stacks;
      const float radius = radiusBase * (1.0 - jDivStacks) + radiusTop * jDivStacks;

      // Position
      verts[index++]  = xDir * radius;
      verts[index++]  = y0 + jDivStacks * height;
      verts[index++]  = zDir * radius;

      // Normal
      verts[index++]  = xDir;
      verts[index++]  = 0.0;
      verts[index++]  = zDir;

      // Texture coordinates
      verts[index++]  = iDivSlices;
      verts[index++]  = jDivStacks;
    }
  }

  // Build triangles
  const unsigned int triCount = slices * stacks * 2;
  elementCount = triCount * 3;
  GLushort tris[elementCount];
  index = 0;
  for (unsigned int i = 0; i < slices; i++)
    for (unsigned int j = 0; j < stacks; j++)
    {
      // Vertex indices
      const unsigned int v0 = i * stackVerts + j;
      const unsigned int v1 = v0 + stackVerts;
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

SimpleCylinder::~SimpleCylinder()
{
  glDeleteBuffers(1, &ibo);
  glDeleteBuffers(1, &vbo);
}

void SimpleCylinder::Render() const
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
