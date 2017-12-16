#include "../Quadtree/Math/VectorMath.h"

#ifndef PARTICLE_H

#define PARTICLE_H

struct Particle {
	Particle() :
			position(0.0f), velocity(0.0f), color(0.0f), age(0.0f), lifeTime(
					0.0f), speed(0.0f) {
	}

//    glm::vec3   position;
//    glm::vec3   velocity;
//    glm::vec4   color;
	Vector3d position;
	Vector3d velocity;
	Vector4d color;
	float speed;
	float size;
	float age;
	float lifeTime;
	float cameradistance;
};

typedef std::vector<Particle> ParticleBuffer;
#endif // PARTICLE_H
