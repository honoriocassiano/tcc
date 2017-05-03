#include "AstronomicalObject.h"

bool AstronomicalObject::translateWhenRendering = true;

void AstronomicalObject::SetTranslateWhenRendering(const bool value)
{
  translateWhenRendering = value;
}

bool AstronomicalObject::GetTranslateWhenRendering()
{
  return translateWhenRendering;
}

AstronomicalObject::AstronomicalObject()
  : parent(0), name(), mass(1.0)
{
}

AstronomicalObject::~AstronomicalObject()
{
  // Detach children (attach to parent if available)
  while (children.size())
    children.front()->SetParent(parent ? parent : 0);

  // Detach from parent
  if (parent)
    SetParent(0);
}

void AstronomicalObject::SetParent(AstronomicalObject *newParent)
{
  // Detach from previous parent
  if (parent)
    for (unsigned int i = 0; i < parent->children.size(); i++)
      if (parent->children[i] == this)
      {
        parent->children.erase(parent->children.begin() + i);
        break;
      }

  // Attach new parent
  if (newParent)
    newParent->children.push_back(this);
  parent = newParent;
}

double AstronomicalObject::GetHoverBoundingRadius() const
{
  return GetBoundingRadius();
}

const AstronomicalObject* AstronomicalObject::GetParent() const
{
  return parent;
}

string AstronomicalObject::GetName() const
{
  return name;
}

void AstronomicalObject::SetName(const string &name)
{
  this->name = name;
}

double AstronomicalObject::GetMass() const
{
  return mass;
}

void AstronomicalObject::SetMass(const double mass)
{
  this->mass = mass;
}

// Returns the parent's position (our local origin)
Vector3<double> AstronomicalObject::GetParentPosition() const
{
  return parent ? parent->GetPosition() : Vector3<double>(0.0, 0.0, 0.0);
}

string AstronomicalObject::GetString() const
{
  return name.length() ? name : "Uncharted Astronomical Object";
}
