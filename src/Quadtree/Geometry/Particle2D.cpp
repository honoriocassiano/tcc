#include "Particle2D.h"
#include <GL/gl.h>

// Linked list top node
Particle2D* Particle2D::firstParticle = 0;

Particle2D::Particle2D(const Vector3<float> &position, const Vector3<float> &velocity, const float friction, const float rotation, const float angularVelocity, const float rotationFriction, const float scale, const float scaleDecay, const float life)
  : prevParticle(0), nextParticle(0), position(position), velocity(velocity), friction(friction), rotation(rotation), angularVelocity(angularVelocity), rotationFriction(rotationFriction), scale(scale), scaleDecay(scaleDecay), life(life)
{
  // Set the new particle as the first particle (the previous first becomes second in the list)
  if (firstParticle)
  {
    nextParticle = firstParticle;
    firstParticle->prevParticle = this;
  }
  firstParticle = this;
}

// Destructor
Particle2D::~Particle2D()
{
  // If we're removing the first particle in the list
  if (this == firstParticle)
    firstParticle = nextParticle;

  // Connect next and prev
  if (prevParticle)
    prevParticle->nextParticle = nextParticle;
  if (nextParticle)
    nextParticle->prevParticle = prevParticle;
}

Matrix4x4<float> Particle2D::GetCurrentBillboardingMatrix()
{
  // Modify the current modelview matrix to achieve billboarding (transpose upper left 3x3 submatrix)
  GLfloat modelview[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
  modelview[3] = modelview[7] = modelview[11] = modelview[12] = modelview[13] = modelview[14] = 0.0f;
  modelview[15] = 1.0f;
  const float mv1 = modelview[1];
  const float mv2 = modelview[2];
  const float mv6 = modelview[6];
  modelview[1] = modelview[4];
  modelview[2] = modelview[8];
  modelview[6] = modelview[9];
  modelview[4] = mv1;
  modelview[8] = mv2;
  modelview[9] = mv6;
  return Matrix4x4<float>(modelview);
}

void Particle2D::DeleteAllParticles()
{
  // Loop through the linked list
  Particle2D *particle = firstParticle;
  while (particle)
  {
    // Get the next particle and delete the current one
    Particle2D *nextParticle = particle->nextParticle;
    delete particle;
    particle = nextParticle;
  }
}

void Particle2D::RenderAllParticles()
{
  // Prepare to render
  glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // Get billboarding matrix (same for all particles)
  glMatrixMode(GL_MODELVIEW);
  const Matrix4x4<float> bb = GetCurrentBillboardingMatrix();

  // Loop through the linked list
  Particle2D *particle = firstParticle;
  while (particle)
  {
    // Set modelview matrix
    glPushMatrix();
    glTranslatef(particle->position.x, particle->position.y, particle->position.z);
    glMultMatrixf(bb.data1d);
    glScalef(particle->scale, particle->scale, particle->scale);
    glRotatef(particle->rotation, 0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, particle->life);

    // Render a square with texture coordinates
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2i(0,  1);
    glVertex2i( -1, -1);
    glTexCoord2i(1,  1);
    glVertex2i(  1, -1);
    glTexCoord2i(0,  0);
    glVertex2i( -1,  1);
    glTexCoord2i(1,  0);
    glVertex2i(  1,  1);
    glEnd();

    // Restore modelview matrix
    glPopMatrix();

    // Step to the next particle
    particle = particle->nextParticle;
  }

  glPopAttrib();
}

void Particle2D::UpdateAllParticles(const float time)
{
  // Loop through the linked list
  Particle2D *particle = firstParticle;
  while (particle)
  {
    // Get the next particle here (in case the current one is deleted)
    Particle2D *nextParticle = particle->nextParticle;

    // Drain life
    particle->life -= time;

    // If out of life; delete the particle
    if (particle->life <= 0.0)
      delete particle;
    else
    {
      // Update position and rotation
      particle->position += particle->velocity                                            * time;
      particle->rotation += particle->angularVelocity                                     * time;

      // Apply friction
      particle->velocity        -= particle->velocity        * particle->friction         * time;
      particle->angularVelocity -= particle->angularVelocity * particle->rotationFriction * time;

      // Apply scale decay (similar to friction)
      particle->scale           -= particle->scale           * particle->scaleDecay       * time;
    }

    // Step to the next particle
    particle = nextParticle;
  }
}
