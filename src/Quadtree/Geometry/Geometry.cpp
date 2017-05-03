#include "Geometry.h"
#include <GL/glu.h>

Geometry::Geometry(const Vector3<double> &position, const Matrix3x3<double> &orientation)
  : position(position), orientation(orientation)
{
}

Vector2<int> Geometry::GetScreenPosition(const bool flipY, const bool useOrigin) const
{
  // Get matrices and viewport
  GLdouble  modelview[16], projection[16];
  GLint     viewport[4];
  glGetDoublev(GL_MODELVIEW_MATRIX,   modelview);
  glGetDoublev(GL_PROJECTION_MATRIX,  projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Project to screen
  Vector3<GLdouble> worldCoordinates = useOrigin ? Vector3<double>(0.0, 0.0, 0.0) : position;
  Vector3<GLdouble> screenCoordinates;
  if (!gluProject(worldCoordinates.x, worldCoordinates.y, worldCoordinates.z, modelview, projection, viewport, &screenCoordinates.x, &screenCoordinates.y, &screenCoordinates.z))
    return Vector2<int>(0, 0);

  // If the y axis should be flipped to use top-left origin
  if (flipY)
    screenCoordinates.y = viewport[3] - screenCoordinates.y;

  // Return x and y screen coordinates
  return Vector2<int>((int)screenCoordinates.x, (int)screenCoordinates.y);
}

Vector3<double> Geometry::GetPosition() const
{
  return position;
}

void Geometry::SetPosition(const Vector3<double> &position)
{
  this->position = position;
}

Matrix3x3<double> Geometry::GetOrientation() const
{
  return orientation;
}

void Geometry::SetOrientation(const Matrix3x3<double> &orientation)
{
  this->orientation = orientation;
}

Vector3<double> Geometry::TransformToLocal(const Vector3<double> &v, const bool translate) const
{
  return orientation.GetTranspose() * (translate ? (v - position) : v);
}

Vector3<double> Geometry::TransformToWorld(const Vector3<double> &v, const bool translate) const
{
  return (translate ? position : 0.0) + orientation * v;
}
