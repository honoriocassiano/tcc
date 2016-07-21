/*
 * Landscape.h
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_LANDSCAPE_H_
#define SRC_LANDSCAPE_H_

#include "BTTreeNode.h"
#include "Patch.h"
#include "Utils.h"

class Landscape {
public:
	Landscape();
	virtual ~Landscape();

	void render();
	void computeVariance();

	void toggleWireframe();

	void tessellate(const Vec3f& cameraPosition);

	const Patch* getPatch() const { return mPatch; }

private:
	Patch* mPatch;
};

#endif /* SRC_LANDSCAPE_H_ */
