//#include "precompiledheader.h"
#include "StarSystem.h"

StarSystem::StarSystem(Camera<double> *_camera) :
		camera(_camera) {
}
;

bool comp(Star *s1, Star *s2) {
	return s1->cameraDistance > s2->cameraDistance;
}

void StarSystem::update(GLfloat deltaTime) {
	for (int i = 0; i < stars.size(); i++) {
		stars[i]->update(deltaTime);
	}
	std::sort(stars.begin(), stars.end(), comp);
}

void StarSystem::render() {
	for (int i = 0; i < stars.size(); i++) {
		stars[i]->render();
	}
}

void StarSystem::addStar(Star *s) {
	stars.push_back(s);

	if (s->effects.size() == 0) {
		printf(
				"ERRO em StarSystem::addStar(): a estrela a ser adicionada n�o tem efeitos de part�cula.");
		exit(-1);
	}

	for (int i = 0; i < s->effects.size(); i++)
		s->effects[i]->setCamera(camera);
}

Star *StarSystem::getStar(unsigned int index) {
	return stars[index];
}
