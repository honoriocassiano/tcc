#include "../Quadtree/Math/Randomizer.h"

#include "CubeEmitter.h"

#include <GL/gl.h>

CubeEmitter::CubeEmitter() :
		MinWidth(-1), MaxWidth(1), MinHeight(-1), MaxHeight(1), MinDepth(-1), MaxDepth(
				1), MinSpeed(10), MaxSpeed(20), MinLifetime(3), MaxLifetime(5), Origin(
				0.0f) {
}

void CubeEmitter::EmitParticle(Particle& particle) {
	float X = Randomizer::GetFloat(MinWidth, MaxWidth);
	float Y = Randomizer::GetFloat(MinHeight, MaxHeight);
	float Z = Randomizer::GetFloat(MinDepth, MaxDepth);

	float lifetime = Randomizer::GetFloat(MinLifetime, MaxLifetime);
	float speed = Randomizer::GetFloat(MinSpeed, MaxSpeed);

	Vector3d vector(X, Y, Z);

	particle.position = vector + Origin;
	particle.velocity = vector.Normalize() * speed;

	particle.lifeTime = lifetime;
	particle.age = 0;
}

void CubeEmitter::DebugRender() {
	glTranslated(Origin.x, Origin.y, Origin.z);

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

//    glutWireCube( MaxWidth * 2 );

	glTranslated(-Origin.x, -Origin.y, -Origin.z);
}
