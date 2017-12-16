#include "Star.h"

Star::Star(float radius, Vector3d position, Vector3d _sphereColor) :
		initializer(radius, position), sphereColor(_sphereColor) {
}

void Star::addParticleEffects(const char *textureFilename,
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

void Star::update(GLfloat deltaTime) {
	GLfloat _mvmatrix[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, _mvmatrix);
	Matrix4x4f mvmatrix(_mvmatrix);
//    memcpy(glm::value_ptr(mvmatrix), _mvmatrix, sizeof( _mvmatrix ) );

//    Vector4f camPosition = glm::inverse(mvmatrix)[3];
	Vector4f camPosition = mvmatrix.Invert()[3];

//    cameraDistance = glm::length2(glm::vec4(initializer.origin, 1) - camPosition);
	cameraDistance = (Vector4f(initializer.origin.x, initializer.origin.y,
			initializer.origin.z, 1) - camPosition).GetLength();

	for (int i = 0; i < effects.size(); i++) {
		effects[i]->update(deltaTime);
	}
}

void Star::render() {
	for (int i = 0; i < effects.size(); i++)
		effects[i]->render();
}

ParticleEffects *Star::getEffects(int index) {
	return effects[index];
}

