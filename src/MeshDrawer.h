/*
 * MeshDrawer.h
 *
 *  Created on: 4 de mai de 2016
 *      Author: cassiano
 */

#ifndef SRC_MESHDRAWER_H_
#define SRC_MESHDRAWER_H_

#include "mesh.h"

typedef struct _do {
	bool wireframe;
	bool normals;
	bool noise;

	_do() : wireframe(false), normals(false), noise(false) {}

} DrawOptions;

class MeshDrawer {
public:
	MeshDrawer();
	virtual ~MeshDrawer();

//	static void draw(Mesh * mesh, bool wireframe = false, bool generateNoise = true);

	static void draw(Mesh * mesh, const DrawOptions& options = DrawOptions());

private:
	static void drawNormal(Triangle* triangle);
	static Vec3f getCentroid(const Triangle* triangle);

	static bool isClockwise(const Triangle* triangle);
};

#endif /* SRC_MESHDRAWER_H_ */
