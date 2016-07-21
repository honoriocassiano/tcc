/*
 * Landscape.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Landscape.h"

Landscape::Landscape() : mPatch(new Patch()) {

}

Landscape::~Landscape() {
	delete mPatch;
}

void Landscape::render() {

	int numTriangles = mPatch->getMesh()->numTriangles();

	mPatch->render();

	if(numTriangles <= maxTriangles) {
		frameVariance += ((float)(numTriangles - maxTriangles))/ maxTriangles;
	}

	if(frameVariance < 0) {
		frameVariance = 0;
	}
}

// TODO Check this method
void Landscape::tessellate(const Vec3f& cameraPosition) {
	mPatch->tessellate(cameraPosition);
}
