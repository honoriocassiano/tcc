#include "ParticleEffects.h"
#include "ParticleInitializer.h"
#include "../Quadtree/Math/Randomizer.h"
#include "SOIL/SOIL.h"
#include "GL/glut.h"

#include <cassert>
#include <iostream>

ParticleEffects::ParticleEffects(GLuint numParticles, float minLifeTime,
		float maxLifeTime) :
		camera(NULL), particleInitializer(NULL), colorInterpolator(
				Vector4d(1.0f)), textureID(0), sphereColor(Vector3d(0.0f)), particleInitialSize(
				1), particleFinalSize(0), minLifeTime(minLifeTime), maxLifeTime(
				maxLifeTime), minSpeed(1.0f), maxSpeed(2.0f) {
	particles.resize(numParticles, Particle());
	vertexBuffer.resize(numParticles * 4, Vertex());
}

ParticleEffects::~ParticleEffects() {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
}

void ParticleEffects::setCamera(Camera<double>* camera) {
	this->camera = camera;
}

void ParticleEffects::setParticleInitializer(
		ParticleInitializer* pInitializer) {
	particleInitializer = pInitializer;
}

void ParticleEffects::setColorInterplator(const ColorInterpolator& colors) {
	colorInterpolator = colors;
}

bool ParticleEffects::loadTexture(const std::string& fileName) {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
	}

	textureID = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!textureID) {
		std::cout << "Cannot load texture " << fileName << std::endl;
	}

	return (textureID != 0);
}

void ParticleEffects::setParticleSpeedRange(float minSpeed, float maxSpeed) {
	this->minSpeed = minSpeed <= maxSpeed ? minSpeed : maxSpeed;
	this->maxSpeed = minSpeed >= maxSpeed ? minSpeed : maxSpeed;
}

void ParticleEffects::initParticle(Particle& particle) {
	assert(particleInitializer != NULL);
	float lifetime = Randomizer::GetFloat(minLifeTime, maxLifeTime);
	particle.lifeTime = lifetime;
	float speed = Randomizer::GetFloat(minSpeed, maxSpeed);
	particle.speed = speed;
	particleInitializer->initParticle(particle);
}

void ParticleEffects::initParticles() {
	for (unsigned int i = 0; i < particles.size(); ++i) {
		initParticle(particles[i]);
	}
}

void ParticleEffects::setSphereColor(Vector3d color) {
	sphereColor = color;
}

void ParticleEffects::renderSphere() const {
	float radius = (particleInitializer->maximumRadius
			+ particleInitializer->minimumRadius) / 2.0f;
	Vector3d position = particleInitializer->origin;

//    position += camera->getTranslation() * camera->getRotation();
	position += camera->GetPosition() * camera->GetForwardVector();

	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glColor3d(sphereColor.x, sphereColor.y, sphereColor.z);

	// TODO Add sphere here
	glutSolidSphere(radius, 32, 32);

	glPopMatrix();
	glPopAttrib();
}

void ParticleEffects::buildVertexBuffer() {
	const Vector3d x(0.5, 0, 0);
	const Vector3d y(0, 0.5, 0);

//    glm::quat cameraRotation;
//	Vector3d cameraRotation;
	Matrix3x3d cameraRotation;

	if (camera != NULL) {
//        cameraRotation = glm::quat( glm::radians(camera->getRotation()) );
//    	cameraRotation = glm::quat( glm::radians(camera->GetLeftVector()Rotation()) );
//		cameraRotation = camera->GetForwardVector();
		cameraRotation = camera->GetOrientation();

		for (unsigned int i = 0; i < particles.size(); ++i) {
			Particle& particle = particles[i];

			unsigned int vertexIndex = i * 4;
			Vertex& v0 = vertexBuffer[vertexIndex + 0];   //Inferior esquerdo
			Vertex& v1 = vertexBuffer[vertexIndex + 1];   //Inferior direito
			Vertex& v2 = vertexBuffer[vertexIndex + 2];   //Superior direito
			Vertex& v3 = vertexBuffer[vertexIndex + 3];   //Superior esquerdo

			//Inferior esquerdo
			v0.pos = cameraRotation * particle.position
					+ ((-x - y) * particle.size);
			v0.tex0 = Vector2f(0, 1);
			v0.diffuse = particle.color;

			//Inferior direito
			v1.pos = cameraRotation * particle.position
					+ ((x - y) * particle.size);
			v1.tex0 = Vector2f(1, 1);
			v1.diffuse = particle.color;

			//Superior direito
			v2.pos = cameraRotation * particle.position
					+ ((x + y) * particle.size);
			v2.tex0 = Vector2f(1, 0);
			v2.diffuse = particle.color;

			//Superior esquerdo
			v3.pos = cameraRotation * particle.position
					+ ((-x + y) * particle.size);
			v3.tex0 = Vector2f(0, 0);
			v3.diffuse = particle.color;
		}
	}
}

void ParticleEffects::setParticleSizeRange(float initialSize, float finalSize) {
	particleInitialSize = initialSize;
	particleFinalSize = finalSize;
}

void ParticleEffects::update(float deltaTime) {

	for (unsigned int i = 0; i < particles.size(); ++i) {
		Particle& particle = particles[i];

		particle.age += deltaTime;
		if (particle.age > particle.lifeTime) {
			initParticle(particle);
		}

		float lifeRatio = particle.age / particle.lifeTime;
		particle.position += (particle.velocity * deltaTime);
		particle.color = colorInterpolator.getValue(lifeRatio);
//        particle.size = glm::lerp<float>( particleInitialSize, particleFinalSize, lifeRatio);
		particle.size = particleInitialSize
				+ lifeRatio * (particleFinalSize - particleInitialSize);

//		std::printf("(%lf, %lf, %lf)\n", particle.position.x, particle.position.y, particle.position.z);
	}

	buildVertexBuffer();
}

void ParticleEffects::render() const {
	renderSphere();

	glEnable(GL_BLEND);                 //Habilida blending
	glDepthMask(GL_FALSE); //N�o permite que as part�culas escrevam no buffer de profundidade
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Chamado de True Transparency
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	glBindTexture( GL_TEXTURE_2D, textureID);

	glEnableClientState( GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY);
	glEnableClientState( GL_COLOR_ARRAY);

//	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(vertexBuffer[0].pos));
//	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(vertexBuffer[0].tex0));
	glVertexPointer(3, GL_DOUBLE, sizeof(Vertex), &(vertexBuffer[0].pos));
	glTexCoordPointer(2, GL_DOUBLE, sizeof(Vertex), &(vertexBuffer[0].tex0));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &(vertexBuffer[0].diffuse));

	glDrawArrays( GL_QUADS, 0, vertexBuffer.size());

	glDisableClientState( GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY);

	glBindTexture( GL_TEXTURE_2D, 0);

	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
