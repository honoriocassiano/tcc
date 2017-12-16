#ifndef PARTICLE_EFFECTS2_H

#define PARTICLE_EFFECTS2_H

#define GL_GLEXT_PROTOTYPES
#include "../Quadtree/GL/GLee.h"
#include <GL/gl.h>

#include "Interpolator.h"
#include "ParticleInitializer.h"
#include "Particle.h"

#include "../Quadtree/Application/Camera.h"

#ifdef NUM_FBOS
#undef NUM_FBOS
#endif

#define NUM_FBOS 1

class ParticleEffects2 {
public:

	struct Vertex {
		Vertex() :
				pos(0.0f), diffuse(0.0f), tex0(0.0f) {
		}

		Vector3d pos;
		Vector4d diffuse;
		Vector2f tex0;
	};

	typedef std::vector<Vertex> VertexBuffer;
	typedef Interpolator<Vector4d> ColorInterpolator;

	ParticleEffects2(GLsizei numParticles, float minLifeTime, float maxLifeTime, ParticleInitializer* initializer = nullptr);
	virtual ~ParticleEffects2();

	void setCamera(Camera<double>* camera);
	void setParticleInitializer(ParticleInitializer *initializer);
	void setColorInterplator(const ColorInterpolator& colors);

	void initParticles();

	virtual void update(float deltaTime);
	virtual void render()  const;

	bool loadTexture(const std::string& fileName);

	void buildVertexBuffer();
	void setSphereColor(Vector3d color);
	void setParticleSizeRange(float particleInitialSize,
			float particleFinalSize);
	void setParticleSpeedRange(float minSpeed, float maxSpeed);

protected:
	void initParticle(Particle& particle);
	void initParticleAt(std::size_t position);

	void initTextures();

	static GLuint fbos[NUM_FBOS];

private:
	float minLifeTime;
	float maxLifeTime;
	float minSpeed;
	float maxSpeed;
	float particleInitialSize;
	float particleFinalSize;
	void renderSphere() const;
	Vector3d sphereColor;
	Camera<double>* camera;
	ParticleInitializer* particleInitializer;
	ColorInterpolator colorInterpolator;
	ParticleBuffer particles;
	VertexBuffer vertexBuffer;
	GLuint textureID;

	GLsizei numParticles;

	GLuint positionID;
	GLuint speedID;

	// al = Age, Lifetime
	GLuint alID;
//	GLuint ageID;
//	GLuint lifetimeID;
//
//	GLuint ageBufferID;
//	GLuint lifetimeBufferID;

};
#endif // PARTICLE_EFFECTS_H
