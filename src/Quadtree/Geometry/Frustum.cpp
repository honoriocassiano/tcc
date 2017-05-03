#include "Frustum.h"
#include <GL/gl.h>

// Creates a frustum based on current OpenGL matrices
Frustum::Frustum()
{
  // Calculate the modelview-projection matrix
  Matrix4x4<double> projection;
  Matrix4x4<double> modelview;
  glGetDoublev(GL_PROJECTION_MATRIX, projection.data1d);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data1d);
  const Matrix4x4<double> mvp = modelview * projection;

  // Extract the clipping planes; right, left, far, near, top, bottom
  planes[0] = mvp.GetColumn(3) - mvp.GetColumn(0);
  planes[1] = mvp.GetColumn(3) + mvp.GetColumn(0);
  planes[2] = mvp.GetColumn(3) - mvp.GetColumn(2);
  planes[3] = mvp.GetColumn(3) + mvp.GetColumn(2);
  planes[4] = mvp.GetColumn(3) - mvp.GetColumn(1);
  planes[5] = mvp.GetColumn(3) + mvp.GetColumn(1);

  // Normalize the clipping planes
  NormalizePlanes();

  /*
  printf("FRUSTUM:\n[%.2f, %.2f, %.2f, %.2f]\n[%.2f, %.2f, %.2f, %.2f]\n[%.2f, %.2f, %.2f, %.2f]\n[%.2f, %.2f, %.2f, %.2f]\n[%.2f, %.2f, %.2f, %.2f]\n[%.2f, %.2f, %.2f, %.2f]\n",
    planes[0][0], planes[0][1], planes[0][2], planes[0][3],
    planes[1][0], planes[1][1], planes[1][2], planes[1][3],
    planes[2][0], planes[2][1], planes[2][2], planes[2][3],
    planes[3][0], planes[3][1], planes[3][2], planes[3][3],
    planes[4][0], planes[4][1], planes[4][2], planes[4][3],
    planes[5][0], planes[5][1], planes[5][2], planes[5][3]);
    */
}

// Calculates the closest distance from a given point to a given clipping plane
double Frustum::CalculateDistanceToPlane(const int plane, const Vector3<double> &point) const
{
  // Return the point-to-plane distance
  return planes[plane].x * point.x + planes[plane].y * point.y + planes[plane].z * point.z + planes[plane].w;
}

// Normalizes the clipping planes
void Frustum::NormalizePlanes()
{
  // For each plane
  for (unsigned int i = 0; i < 6; i++)
  {
    // Normalize the plane using the length of its vector part
    const double normalLength = Vector3<double>(planes[i].x, planes[i].y, planes[i].z).GetLength();
    if (normalLength != 0.0)
      planes[i] /= normalLength;
  }
}

// Determines whether a given point is inside the frustum
bool Frustum::ContainsPoint(const Vector3<double> &point) const
{
  // For each plane; return outside if the point is behind the plane
  for (unsigned int i = 0; i < 6; i++)
    if (CalculateDistanceToPlane(i, point) <= 0.0)
      return OUTSIDE;

  // Return inside
  return INSIDE;
}

// Determines whether a given sphere is inside the frustum
int Frustum::ContainsSphere(const Vector3<double> &position, const double radius) const
{
  // Plane counter
  int planeCount = 0;

  // Use the point-to-plane distance to calculate the number of planes the sphere is in front of
  for (unsigned int i = 0; i < 6; i++)
  {
    const double distance = CalculateDistanceToPlane(i, position);
    if (distance <= -radius)
      return OUTSIDE;
    else if (distance > radius)
      planeCount++;
  }

  // Return inside if in front of all planes; otherwise intersecting
  return planeCount == 6 ? INSIDE : INTERSECTING;
}

// Determines whether a given box is inside the frustum
int Frustum::ContainsBox(const Vector3<double> &min, const Vector3<double> &max) const
{
  // Build a vector holding all box corners
  vector<Vector3<double> > points;
  for (unsigned int i = 0; i < 8; i++)
    points.push_back(Vector3<double>(i < 4 ? max.x : min.x, i % 4 < 2 ? max.y : min.y, i % 2 ? max.z : min.z));

  // Test the box as a polygon
  return ContainsPolygon(points);
}

// Determines whether a given polygon is inside the frustum
int Frustum::ContainsPolygon(const vector<Vector3<double> > &points) const
{
  // Plane counter
  int planeCount = 0;

  // Use the point-to-plane distance to calculate the number of planes the polygon is in front of
  for (unsigned int i = 0; i < 6; i++)
  {
    unsigned int pointCount = 0;
    for (unsigned int j = 0; j < points.size(); j++)
      if (CalculateDistanceToPlane(i, points[j]) > 0.0)
        pointCount++;
    if (pointCount == 0)
      return OUTSIDE;
    else if (pointCount == points.size())
      planeCount++;
  }

  // Return inside if in front of all planes; otherwise intersecting
  return planeCount == 6 ? INSIDE : INTERSECTING;
}
