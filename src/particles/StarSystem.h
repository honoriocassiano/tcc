#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

#include "Star.h"
#include "../Quadtree/Application/Camera.h"

class StarSystem {
public:
	StarSystem(Camera<double> *_camera);
	void update(GLfloat deltaTime);
	void render();
	void addStar(Star *s);
	Star *getStar(unsigned int index);
private:
	std::vector<Star*> stars;
	Camera<double> *camera;
};

#endif // STAR_SYSTEM_H
