#include "ParticleInitializer.h"
#include "../Quadtree/Math/Randomizer.h"
#include "../Quadtree/Math/VectorMath.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944

ParticleInitializer::ParticleInitializer(GLfloat radius, Vector3d _origin) :
		minimumRadius(radius), maximumRadius(radius), origin(_origin) {
}

void ParticleInitializer::initParticle(Particle& particle) {
	std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> random(0, 1);

	float inclination = acos(2 * random(gen) - 1);
	float azimuth = 2 * PI * random(gen);
	float radius = Randomizer::GetFloat(minimumRadius, maximumRadius);

	float sinInclination = sinf(inclination);

	float X = sinInclination * cosf(azimuth);
	float Y = sinInclination * sinf(azimuth);
	float Z = cosf(inclination);

	Vector3d vector(X, Y, Z);

	particle.position = (vector * radius) + origin;
	particle.velocity = vector * particle.speed;

	particle.age = 0;
}
