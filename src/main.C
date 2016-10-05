#include "Debug.h"

#include <GL/glut.h>
#include "PriorityQueue.h"
#include "Planet.h"

#include "SDLWindow.h"

#include "Patch.h"
#include "Landscape.h"

#include "Object.h"
#include "Quadtree.h"

// =========================================
// =========================================

int main(int argc, char **argv) {
	srand(time(NULL));

	SDLWindow window(640, 480);

	auto tree = getTree();

	auto direction = directions::Intercardinal::NW;

	tree->subdivide(direction);
	tree->getChild(direction)->subdivide(directions::Intercardinal::SE);

	window.run();
	/*
	 SDLWindow window(640, 480);

	 Planet* planet1 = new Planet(5);
	 planet1->setCenter(Vec3f(50, 0, 0));
	 planet1->setMass(5);

	 Planet* planet2 = new Planet(5);
	 planet2->setCenter(Vec3f(100, 0, 0));
	 planet2->setMass(5);

	 Planet* planet3 = new Planet(5);
	 planet3->setCenter(Vec3f(200, 0, 0));
	 planet3->setMass(5);

	 Planet* sun = new Planet(10);
	 sun->setCenter(Vec3f(0, 0, 0));
	 sun->setMass(10000);

	 planet1->setOrbiter(sun);
	 planet2->setOrbiter(sun);
	 planet3->setOrbiter(sun);

	 window.addBody(planet1);
	 window.addBody(planet2);
	 window.addBody(planet3);
	 window.addBody(sun);

	 window.run();
	 */

	/*
	 Quadtree* tree = new Quadtree(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), 0, 20);

	 Log("%d, %d", tree->getMesh()->numTriangles(), tree->getMesh()->numVertices());

	 auto direction = directions::Intercardinal::NW;

	 tree->subdivide(direction);

	 Log("%d, %d", tree->getMesh()->numTriangles(), tree->getMesh()->numVertices());

	 //	tree->merge(direction);
	 //
	 //	Log("%d, %d", tree->getMesh()->numTriangles(), tree->getMesh()->numVertices());

	 //	tree->getMesh()->printVertices();
	 tree->getMesh()->printTriangles();

	 delete tree;
	 */
	/*
	 tree->AddObject(new Object(25, 25, 1, 1));

	 for (int i = 0; i < 20; ++i) {
	 Log("%d: %lu", i, tree->GetObjectsAt(i, i).size());
	 }
	 */

//	Patch* patch = new Patch(0.25);
//	Mesh* mesh = patch->getMesh();
//
//	Log("original - triangles: %d, vertices: %d", mesh->numTriangles(),
//			mesh->numVertices());
//
//	patch->processGeometry(Vec3f(0, 0, 0.5), Vec3f(0, 0, -1));
//
//	Log("after process 1 - triangles: %d, vertices: %d", mesh->numTriangles(),
//			mesh->numVertices());
//
//	patch->processGeometry(Vec3f(0, 0, 2.0), Vec3f(0, 0, -1));
//
//	Log("after process 2 - triangles: %d, vertices: %d", mesh->numTriangles(),
//			mesh->numVertices());
//********************************************
	/*

	 Patch* patch = new Patch(1.0);

	 Mesh* mesh = patch->getMesh();

	 //	auto v0 = mesh->addVertex(Vec3f(0, 0, 0));
	 //	auto v1 = mesh->addVertex(Vec3f(1, 0, 0));
	 //	auto v2 = mesh->addVertex(Vec3f(0, 1, 0));
	 //
	 //	auto tri = mesh->addTriangle(v0, v1, v2);

	 Log("original - triangles: %d, vertices: %d", mesh->numTriangles(),
	 mesh->numVertices());

	 mesh->printTriangles();

	 //	BTTreeNode* node = new BTTreeNode(tri);
	 BTTreeNode* left = patch->getLeft();
	 BTTreeNode* right = patch->getRight();

	 patch->computeVariance();

	 Log("left: %p, priority: %f", left, left->getPriority());
	 Log("right: %p, priority: %f", right, right->getPriority());

	 patch->split(right);
	 patch->computeVariance();
	 Log("SPLIT!");

	 //	Log("split - triangles: %d, vertices: %d", mesh->numTriangles(),
	 //				mesh->numVertices());

	 Log("left: %p, priority: %f", left, left->getPriority());
	 Log("right: %p, priority: %f", right, right->getPriority());

	 Log("right-l: %p, priority: %f", right->getLeftChild(),
	 right->getLeftChild()->getPriority());
	 Log("right-r: %p, priority: %f", right->getRightChild(),
	 right->getRightChild()->getPriority());

	 patch->merge(right);
	 patch->computeVariance();

	 Log("MERGE!");
	 //	Log("merge - triangles: %d, vertices: %d", mesh->numTriangles(),
	 //					mesh->numVertices());

	 Log("left: %p, priority: %f", left, left->getPriority());
	 Log("right: %p, priority: %f", right, right->getPriority());

	 */
	//********************************************
	/*
	 patch->split(node);

	 Log("split - triangles: %d, vertices: %d\n\n", mesh->numTriangles(), mesh->numVertices());

	 mesh->printTriangles();

	 patch->merge(node);

	 Log("merge - triangles: %d, vertices: %d\n\n", mesh->numTriangles(), mesh->numVertices());

	 mesh->printTriangles();
	 */

	/*
	 SDLWindow window(640, 480);


	 //	Landscape* landscape = new Landscape();
	 //
	 //	Vec3f position(0.5, 0.5, -10);
	 //
	 //	int i = 0;
	 //
	 //	printf("Started!\n");
	 //
	 //	while(i < 100) {
	 //		landscape->tessellate(position);
	 //		landscape->render();
	 //
	 //		++i;
	 //	}


	 Log("Started\n");

	 window.run();
	 */
	Log("Finished!\n");

	return 0;
}

/*
 int main(int argc, char *argv[]) {
 //srand48(0);
 srand(time(NULL));

 glutInit(&argc, argv);

 ArgParser *args = new ArgParser(argc, argv);

 Planet* sphere = new Planet(5, 150, 150);

 // mesh->Load(args->input_file);

 // mesh->LoopSubdivision();

 // sphere->setCenter(Vec3f(5, -1, 2));

 //GLCanvas glcanvas;
 Window window;

 window.addBody(sphere);

 window.initialize(args, sphere->getMesh());
 //glcanvas.initialize(args, sphere->getMesh());

 //delete args;

 return 0;
 }
 */

// =========================================
// =========================================
