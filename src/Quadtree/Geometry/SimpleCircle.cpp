#include "SimpleCircle.h"
#include <cmath>

#define BUFFER_OFFSET(i) ((char*)0 + (i))
#define VERT_NUM_FLOATS 2

SimpleCircle::SimpleCircle(const float radius, const unsigned int points)
  : vbo(0), ibo(0), elementCount(0)
{
  // Calculate vertex positions
  GLfloat verts[points * VERT_NUM_FLOATS];
  unsigned int index = 0;
  for (unsigned int i = 0; i < points; i++)
  {
    // Calculate angle
    const float alpha  = i / (float)points * 2.0 * M_PI;

    // Set vertex position
    verts[index++] = cos(alpha) * radius;
    verts[index++] = sin(alpha) * radius;
  }

  // Build lines
  elementCount = points;
  GLushort lineStrip[elementCount];
  for (int i = 0; i < elementCount; i++)
    lineStrip[i] = i;

  // Generate and initialize VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERT_NUM_FLOATS * points, verts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate and initialize IBO
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * elementCount, lineStrip, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SimpleCircle::~SimpleCircle()
{
  glDeleteBuffers(1, &ibo);
  glDeleteBuffers(1, &vbo);
}

void SimpleCircle::Render() const
{
  // Bind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * VERT_NUM_FLOATS, BUFFER_OFFSET(0));

  // Render
  glDrawElements(GL_LINE_LOOP, elementCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  // Unbind VBO and IBO
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
}
