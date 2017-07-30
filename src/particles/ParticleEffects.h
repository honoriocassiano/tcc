#ifndef PARTICLE_EFFECTS_H

#define PARTICLE_EFFECTS_H

#include "Interpolator.h"
#include "ParticleInitializer.h"
#include "Particle.h"

#include "../Quadtree/Application/Camera.h"


class ParticleEffects {
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

	ParticleEffects(GLuint numParticles, float minLifeTime, float maxLifeTime);
	virtual ~ParticleEffects();

	void setCamera(Camera<double>* camera);
	void setParticleInitializer(ParticleInitializer *initializer);
	void setColorInterplator(const ColorInterpolator& colors);

	void initParticles();

	virtual void update(float deltaTime);
	virtual void render();

	bool loadTexture(const std::string& fileName);

	void buildVertexBuffer();
	void setSphereColor(Vector3d color);
	void setParticleSizeRange(float particleInitialSize,
			float particleFinalSize);
	void setParticleSpeedRange(float minSpeed, float maxSpeed);

protected:
	void initParticle(Particle& particle);

private:
	float minLifeTime;
	float maxLifeTime;
	float minSpeed;
	float maxSpeed;
	float particleInitialSize;
	float particleFinalSize;
	void renderSphere();
	Vector3d sphereColor;
	Camera<double>* camera;
	ParticleInitializer* particleInitializer;
	ColorInterpolator colorInterpolator;
	ParticleBuffer particles;
	VertexBuffer vertexBuffer;
	GLuint textureID;
};
#endif // PARTICLE_EFFECTS_H
