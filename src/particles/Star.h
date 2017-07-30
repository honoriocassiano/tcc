#ifndef STAR_H
#define STAR_H

#include "ParticleEffects.h"
#include "ParticleInitializer.h"
#include "../Quadtree/Math/VectorMath.h"


class Star {
public:
	ParticleInitializer initializer;
	std::vector<ParticleEffects*> effects;
	Star(float radius, Vector3d position, Vector3d sphereColor);
	void render();
	void addParticleEffects(const char *textureFilename,
			unsigned int nParticles,
			const ParticleEffects::ColorInterpolator& colors,
			float particleInitialSize, float minLifeTime, float maxLifeTime);
	void update(GLfloat deltaTime);
	float cameraDistance;
	ParticleEffects *getEffects(int index);

private:
	Vector3d sphereColor;
};
#endif // STAR_H
