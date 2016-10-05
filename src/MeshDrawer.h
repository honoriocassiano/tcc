/*
 * MeshDrawer.h
 *
 *  Created on: 4 de mai de 2016
 *      Author: cassiano
 */

#ifndef SRC_MESHDRAWER_H_
#define SRC_MESHDRAWER_H_

#include "mesh.h"

class MeshDrawer {
public:
	MeshDrawer();
	virtual ~MeshDrawer();

	static void draw(Mesh * mesh, bool wireframe = false, bool generateNoise = true);
};

#endif /* SRC_MESHDRAWER_H_ */
