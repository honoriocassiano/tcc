/*******************************************************************************
* File:           Particle2D.h
* Author:         Christian Alfons
* Date created:   2011-02-17
* Date modified:  2011-02-17
* Description:    Basic billboard particle class. Uses a linked list to keep
*                 track of all created particles, so they can be updated and
*                 rendered easily.
*******************************************************************************/

#ifndef PARTICLE2D_H_INCLUDED
#define PARTICLE2D_H_INCLUDED

#include "../Math/VectorMath.h"

class Particle2D
{
  private:
    // Class variables
    static Particle2D *firstParticle;

    // Instance variables
    Particle2D *prevParticle, *nextParticle;
    Vector3<float>  position;
    Vector3<float>  velocity;
    float           friction;
    float           rotation;
    float           angularVelocity;
    float           rotationFriction;
    float           scale;
    float           scaleDecay;
    float           life;

  public:
    // Constructor
    Particle2D(const Vector3<float> &position, const Vector3<float> &velocity, const float friction, const float rotation, const float angularVelocity, const float rotationFriction, const float scale, const float scaleDecay, const float life);

    // Destructor
    ~Particle2D();

    // Class methods
    static Matrix4x4<float> GetCurrentBillboardingMatrix();
    static void             DeleteAllParticles();
    static void             RenderAllParticles();
    static void             UpdateAllParticles(const float time);
};

#endif
