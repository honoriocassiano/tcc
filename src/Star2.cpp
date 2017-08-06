/*
 * Star2.cpp
 *
 *  Created on: 3 de ago de 2017
 *      Author: cassiano
 */

#include "Star2.h"

#include "Quadtree/Math/PerlinNoise.h"
#include <sstream>
#include "Constants.h"

namespace pssg {

#define BOUNDING_RADIUS_SCALE 4.4

Star2::Star2(double radius, Vector3d position, Vector3d _sphereColor) :
		radius(radius), initializer(radius, position), sphereColor(_sphereColor) {
}

void Star2::AddParticleEffects(const char *textureFilename,
		unsigned int nParticles,
		const ParticleEffects::ColorInterpolator& colors,
		float particleInitialSize, float minLifeTime, float maxLifeTime) {
	effects.push_back(
			new ParticleEffects(nParticles, minLifeTime, maxLifeTime));
	effects.back()->setParticleInitializer(&initializer);
	effects.back()->loadTexture(textureFilename);
	effects.back()->setColorInterplator(colors);
	effects.back()->setSphereColor(sphereColor);
	effects.back()->setParticleSizeRange(particleInitialSize, 0);
}

Vector3d Star2::GetClosestSurfacePoint(const Vector3d &pos) const {
	return TransformToWorld(TransformToLocal(pos, true).GetNormalized(), false)
			* radius;
}

double Star2::GetHoverBoundingRadius() const {
	return radius;
}

double Star2::GetBoundingRadius() const {
	// Greater than the actual radius, to cover the glow
	return radius * BOUNDING_RADIUS_SCALE;
}

double Star2::GetClosestSurfaceDistance(const Vector3<double> &pos) const {
	return TransformToLocal(pos, true).GetLength() - radius;
}

void Star2::Update(GLfloat deltaTime) {
	GLfloat _mvmatrix[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, _mvmatrix);
	Matrix4x4f mvmatrix(_mvmatrix);
//    memcpy(glm::value_ptr(mvmatrix), _mvmatrix, sizeof( _mvmatrix ) );

//    Vector4f camPosition = glm::inverse(mvmatrix)[3];
	Vector4f camPosition = mvmatrix.Invert()[3];

//    cameraDistance = glm::length2(glm::vec4(initializer.origin, 1) - camPosition);
//	cameraDistance = (Vector4f(initializer.origin.x, initializer.origin.y,
//			initializer.origin.z, 1) - camPosition).GetLength();

	for (int i = 0; i < effects.size(); i++) {
		effects[i]->update(deltaTime);
	}
}

void Star2::Render() const {

//	glPushMatrix();
//	glTranslatef(position.x, position.y, position.z);

	for (int i = 0; i < effects.size(); i++) {
		effects[i]->render();
	}

//	glPopMatrix();
}

ParticleEffects *Star2::GetEffects(int index) {
	return effects[index];
}

} /* namespace pssg */
