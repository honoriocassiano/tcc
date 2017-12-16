#include "ParticleEffects2.h"
#include "ParticleInitializer.h"
#include "../Quadtree/Math/Randomizer.h"
#include <SOIL/SOIL.h>
#include "../Quadtree/Shaders/ShaderManager.h"

#include <cassert>
#include <iostream>

#include <GL/glut.h>

GLuint ParticleEffects2::fbos[NUM_FBOS] = { 0 };

ParticleEffects2::ParticleEffects2(GLsizei numParticles, float minLifeTime,
		float maxLifeTime, ParticleInitializer* initializer) :
		numParticles(numParticles), camera(NULL), particleInitializer(
				initializer), colorInterpolator(Vector4d(1.0f)), textureID(0), sphereColor(
				Vector3d(0.0f)), particleInitialSize(1), particleFinalSize(0), minLifeTime(
				minLifeTime), maxLifeTime(maxLifeTime), minSpeed(1.0f), maxSpeed(
				2.0f) {
	particles.resize(numParticles, Particle());
	vertexBuffer.resize(numParticles * 4, Vertex());

	if (particleInitializer) {
		initParticles();
	}

	initTextures();
}

ParticleEffects2::~ParticleEffects2() {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
}

void ParticleEffects2::setCamera(Camera<double>* camera) {
	this->camera = camera;
}

void ParticleEffects2::setParticleInitializer(
		ParticleInitializer* pInitializer) {
	particleInitializer = pInitializer;
}

void ParticleEffects2::setColorInterplator(const ColorInterpolator& colors) {
	colorInterpolator = colors;
}

bool ParticleEffects2::loadTexture(const std::string& fileName) {
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

void ParticleEffects2::setParticleSpeedRange(float minSpeed, float maxSpeed) {
	this->minSpeed = minSpeed <= maxSpeed ? minSpeed : maxSpeed;
	this->maxSpeed = minSpeed >= maxSpeed ? minSpeed : maxSpeed;
}

void ParticleEffects2::initParticle(Particle& particle) {
	assert(particleInitializer != NULL);
	float lifetime = Randomizer::GetFloat(minLifeTime, maxLifeTime);
	particle.lifeTime = lifetime;
	float speed = Randomizer::GetFloat(minSpeed, maxSpeed);
	particle.speed = speed;
	particleInitializer->initParticle(particle);
}

void ParticleEffects2::initParticles() {
	for (unsigned int i = 0; i < particles.size(); ++i) {
//		initParticle(particles[i]);
		initParticleAt(i);
	}
}

void ParticleEffects2::setSphereColor(Vector3d color) {
	sphereColor = color;
}

void ParticleEffects2::renderSphere() const {
	float radius = (particleInitializer->maximumRadius
			+ particleInitializer->minimumRadius) / 2.0f;
	Vector3d position = particleInitializer->origin;

//    position += camera->getTranslation() * camera->getRotation();
//	position += camera->GetPosition() * camera->GetForwardVector();
//	position += camera->GetOrientation() * camera->GetPosition();

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

void ParticleEffects2::buildVertexBuffer() {
	const Vector3d x(0.5, 0, 0);
	const Vector3d y(0, 0.5, 0);

//    glm::quat cameraRotation;
//	Vector3d cameraRotation;
	Matrix3x3d cameraRotation;

	if (camera) {

//        cameraRotation = glm::quat( glm::radians(camera->getRotation()) );
//    	cameraRotation = glm::quat( glm::radians(camera->GetLeftVector()Rotation()) );
//		cameraRotation = camera->GetForwardVector();

		cameraRotation = camera->GetOrientation();
//		cameraRotation = Matrix3x3d::CreateIdentityMatrix();

		for (unsigned int i = 0; i < particles.size(); ++i) {
			Particle& particle = particles[i];

			unsigned int vertexIndex = i * 4;
			Vertex& v0 = vertexBuffer[vertexIndex + 0];   //Inferior esquerdo
			Vertex& v1 = vertexBuffer[vertexIndex + 1];   //Inferior direito
			Vertex& v2 = vertexBuffer[vertexIndex + 2];   //Superior direito
			Vertex& v3 = vertexBuffer[vertexIndex + 3];   //Superior esquerdo

			//Inferior esquerdo
			v0.pos = particle.position
					+ cameraRotation * ((-x - y) * particle.size);
//			v0.tex0 = Vector2f(0, 1);
			v0.diffuse = particle.color;

			//Inferior direito
			v1.pos = particle.position
					+ cameraRotation * ((x - y) * particle.size);
//			v1.tex0 = Vector2f(1, 1);
			v1.diffuse = particle.color;

			//Superior direito
			v2.pos = particle.position
					+ cameraRotation * ((x + y) * particle.size);
//			v2.tex0 = Vector2f(1, 0);
			v2.diffuse = particle.color;

			//Superior esquerdo
			v3.pos = particle.position
					+ cameraRotation * ((-x + y) * particle.size);
//			v3.tex0 = Vector2f(0, 0);
			v3.diffuse = particle.color;
		}
	}
}

void ParticleEffects2::setParticleSizeRange(float initialSize,
		float finalSize) {
	particleInitialSize = initialSize;
	particleFinalSize = finalSize;
}

void ParticleEffects2::update(float deltaTime) {

//	for (unsigned int i = 0; i < particles.size(); ++i) {
//		Particle& particle = particles[i];
//
//		particle.age += deltaTime;
//		if (particle.age > particle.lifeTime) {
//			initParticle(particle);
//		}
//
//		float lifeRatio = particle.age / particle.lifeTime;
//		particle.position += (particle.velocity * deltaTime);
//		particle.color = colorInterpolator.getValue(lifeRatio);
////        particle.size = glm::lerp<float>( particleInitialSize, particleFinalSize, lifeRatio);
//		particle.size = particleInitialSize
//				+ lifeRatio * (particleFinalSize - particleInitialSize);
//	}
//
//	buildVertexBuffer();

	// Bind the FBO and attach texture to FBO attachment point
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);

//	glBindTexture(GL_TEXTURE_2D, positionID);
//	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D,
//				positionID, 0);

//	// Store render state
	glPushAttrib(GL_LIGHTING_BIT | GL_VIEWPORT_BIT | GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glViewport(0, 0, numParticles, 1);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
//
//	glEnable(GL_TEXTURE_1D);
//	glBindTexture(GL_TEXTURE_1D, positionID);

//	static const GLenum drawbuffers[3] = { GL_COLOR_ATTACHMENT0,
//	GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
////	static const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
////	glDrawBuffers(1, drawbuffers);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
//			positionID, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
//			speedID, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
//			alID, 0);


//	glBindTexture(GL_TEXTURE_2D, positionID);
////		glBindTexture(GL_TEXTURE_2D, positionID);
//
//			glFramebufferTexture2D(
//			    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionID, 0
//			);



	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(
	    GL_TEXTURE_2D, 0, GL_RGB, 1024, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
	glFramebufferTexture2D(
	    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
	);

	ShaderManager::UseShaderProgram(PARTICLE_UPDATE);

//#warning "// TODO Replace to initializer shader"
//	ShaderManager::UseShaderProgram(PATCH_NORMALMAP_HEIGHTMAP);
//
//	static const GLint uniformMinLifetime = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"minLifetime");
//
//	static const GLint uniformMaxLifetime = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"maxLifetime");
//
//	static const GLint uniformInitialSize = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"initialSize");
//
//	static const GLint uniformFinalSize = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"finalSize");
//
//	static const GLint uniformMinSpeed = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"minSpeed");
//
//	static const GLint uniformMaxSpeed = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"maxSpeed");
//
//	glUniform1f(uniformMinLifetime, minLifeTime);
//	glUniform1f(uniformMaxLifetime, maxLifeTime);
//	glUniform1f(uniformInitialSize, particleInitialSize);
//	glUniform1f(uniformFinalSize, particleFinalSize);
//	glUniform1f(uniformMinSpeed, minSpeed);
//	glUniform1f(uniformMaxSpeed, maxSpeed);

	glBegin(GL_QUADS);
	glTexCoord1d(0);
	glVertex2i(-1, -1);

	glTexCoord1d(0);
	glVertex2i(-1, 1);

	glTexCoord1d(1.0);
	glVertex2i(1, -1);

	glTexCoord1d(0);
	glVertex2i(1, 1);

	glEnd();

	// Go back to perspective projection and restore render state
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("FBO ERROR: %d\n", status);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
	glUseProgram(0);
}

void ParticleEffects2::render() const {
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
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(vertexBuffer[0].tex0));
	glColorPointer(4, GL_DOUBLE, sizeof(Vertex), &(vertexBuffer[0].diffuse));

	glDrawArrays( GL_QUADS, 0, vertexBuffer.size());

	glDisableClientState( GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY);

	glBindTexture( GL_TEXTURE_2D, 0);

	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticleEffects2::initParticleAt(std::size_t position) {
	assert(particleInitializer != NULL);

	auto& particle = particles[position];

	auto vertexIndex = position * 4;
	auto speed = Randomizer::GetFloat(minSpeed, maxSpeed);
	auto lifetime = Randomizer::GetFloat(minLifeTime, maxLifeTime);

	particle.lifeTime = lifetime;
	particle.speed = speed;
	particleInitializer->initParticle(particle);

	Vertex& v0 = vertexBuffer[vertexIndex + 0];   //Inferior esquerdo
	Vertex& v1 = vertexBuffer[vertexIndex + 1];   //Inferior direito
	Vertex& v2 = vertexBuffer[vertexIndex + 2];   //Superior direito
	Vertex& v3 = vertexBuffer[vertexIndex + 3];   //Superior esquerdo

	v0.tex0 = Vector2f(0, 1);
	v1.tex0 = Vector2f(1, 1);
	v2.tex0 = Vector2f(1, 0);
	v3.tex0 = Vector2f(0, 0);
}

void ParticleEffects2::initTextures() {

	if (!fbos[0]) {
		glGenFramebuffers(NUM_FBOS, fbos);
	}

	Vector3d positions[numParticles];
	Vector3d speeds[numParticles];
	Vector2f al[numParticles];

	for (auto i = 0; i < particles.size(); ++i) {
		positions[i] = particles[i].position;
		speeds[i] = particles[i].velocity;
		al[i] = Vector2f(particles[i].age, particles[i].lifeTime);
	}

//	// Position texture
//	glGenTextures(1, &positionID);
//	glBindTexture(GL_TEXTURE_1D, positionID);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, numParticles, 0, GL_RGB,
////	GL_DOUBLE, 0);
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, numParticles, 0, GL_RGB,
//	GL_DOUBLE, positions);
	// Position texture
	glGenTextures(1, &positionID);
	glBindTexture(GL_TEXTURE_2D, positionID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, numParticles, 1, 0, GL_RGB,
			GL_UNSIGNED_BYTE, 0);
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, numParticles, 0, GL_RGB,
//	GL_DOUBLE, positions);



	// Position texture
	glGenTextures(1, &speedID);
	glBindTexture(GL_TEXTURE_2D, speedID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, numParticles, 0, GL_RGB,
//	GL_DOUBLE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, numParticles, 1, 0, GL_RGB,
	GL_DOUBLE, speeds);

	// Age-lifetime texture
	glGenTextures(1, &alID);
	glBindTexture(GL_TEXTURE_2D, alID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, numParticles, 0, GL_RG,
//	GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, numParticles, 1, 0, GL_RG,
	GL_FLOAT, al);

//	glBindTexture(GL_TEXTURE_2D, 0);

//	// Bind the FBO and attach texture to FBO attachment point
//	glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
//			positionID, 0);
//
//	// Store render state
//	glPushAttrib(GL_LIGHTING_BIT | GL_VIEWPORT_BIT | GL_ENABLE_BIT);
//	glDisable(GL_LIGHTING);
//	glViewport(0, 0, numParticles, 1);
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//
//	static const GLenum drawbuffers[3] = { GL_COLOR_ATTACHMENT0,
//	GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, drawbuffers);
//	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D,
//			positionID, 0);
//	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_1D,
//			speedID, 0);
//	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_1D,
//			alID, 0);
//
//#warning "// TODO Replace to initializer shader"
//	ShaderManager::UseShaderProgram(PATCH_NORMALMAP_HEIGHTMAP);
//
//	static const GLint uniformMinLifetime = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"minLifetime");
//
//	static const GLint uniformMaxLifetime = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"maxLifetime");
//
//	static const GLint uniformInitialSize = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"initialSize");
//
//	static const GLint uniformFinalSize = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"finalSize");
//
//	static const GLint uniformMinSpeed = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"minSpeed");
//
//	static const GLint uniformMaxSpeed = glGetUniformLocation(
//			ShaderManager::GetShaderProgramHandle(PATCH_NORMALMAP_HEIGHTMAP),
//			"maxSpeed");
//
//	glUniform1f(uniformMinLifetime, minLifeTime);
//	glUniform1f(uniformMaxLifetime, maxLifeTime);
//	glUniform1f(uniformInitialSize, particleInitialSize);
//	glUniform1f(uniformFinalSize, particleFinalSize);
//	glUniform1f(uniformMinSpeed, minSpeed);
//	glUniform1f(uniformMaxSpeed, maxSpeed);
//
//	glBegin(GL_QUADS);
//	glTexCoord1d(0);
//	glVertex2i(-1, -1);
//
//	glTexCoord1d(0);
//	glVertex2i(-1, 1);
//
//	glTexCoord1d(1.0);
//	glVertex2i(1, -1);
//
//	glTexCoord1d(0);
//	glVertex2i(1, 1);
//
//	glEnd();
//
//	// Go back to perspective projection and restore render state
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//	glPopAttrib();
//
//	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (status != GL_FRAMEBUFFER_COMPLETE)
//		printf("FBO ERROR: %d\n", status);
//
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glUseProgram(0);

}
