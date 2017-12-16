#ifndef PARTICLE_INITIALIZER_H

#define PARTICLE_INITIALIZER_H

#include <GL/gl.h>

#include "Particle.h"
#include "../Quadtree/Math/VectorMath.h"

class ParticleInitializer {
public:
	ParticleInitializer(GLfloat radius, Vector3d _origin);
	void initParticle(Particle& particle);

	float minimumRadius;
	float maximumRadius;

	Vector3d origin;
};
#endif // PARTICLE_INITIALIZER_H
