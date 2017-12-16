#include "QuadtreeTerrain.h"

QuadtreeTerrain::QuadtreeTerrain()
  : cameraPosition(0.0, 0.0, 0.0)
{
}

QuadtreeTerrain::~QuadtreeTerrain()
{
  for (unsigned int i = 0; i < faces.size(); i++)
    delete faces[i];
}

void QuadtreeTerrain::Rebuild()
{
  for (unsigned int i = 0; i < faces.size(); i++)
    faces[i]->Rebuild();
}

Vector3<double> QuadtreeTerrain::GetCameraPositionLocal() const
{
  return cameraPosition;
}

void QuadtreeTerrain::SetCameraPosition(const Vector3<double> &cameraPosition)
{
  this->cameraPosition = cameraPosition;
}

const Frustum& QuadtreeTerrain::GetFrustum() const
{
  return frustum;
}

void QuadtreeTerrain::SetFrustum(const Frustum &frustum)
{
  this->frustum = frustum;
}

void QuadtreeTerrain::Render() const
{
  for (unsigned int i = 0; i < faces.size(); i++)
    faces[i]->Render();
}

void QuadtreeTerrain::Update()
{
  for (unsigned int i = 0; i < faces.size(); i++)
    faces[i]->Update();
}
