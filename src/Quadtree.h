#ifndef __QUADTREE_H__
#define __QUADTREE_H__

class Mesh;
class Vertex;

#include "structures/vectors.h"
#include "structures/Halfedge/triangle.h"
#include "structures/Halfedge/mesh.h"

using namespace std;

class Quadtree;
class Object;

namespace directions {
	enum Intercardinal : int {
		NW = 0,
		NE,
		SW,
		SE
	};

	enum Cardinal : int {
		N = 0,
		E,
		S,
		W
	};
}

class Quadtree {
public:
	Quadtree(float x, float y, float width, float height, int level,
			int maxLevel);
	~Quadtree();

	void subdivide(directions::Intercardinal point);

	inline Mesh* getMesh() { return mesh; }

//	void			AddObject(Object *object);
//	vector<Object*>	GetObjectsAt(float x, float y);
//	void			Clear();


protected:

	Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, int level,
			int maxLevel, Mesh * mesh, Quadtree* parent);

private:

	float           x;
	float           y;
	float           width;
	float           height;
	int	            level;
	int             maxLevel;
//	vector<Object*>	objects;

	Vertex* middle;
//	Vertex* nw;
//	Vertex* ne;
//	Vertex* sw;
//	Vertex* se;

	Vertex* vertices[4];
	Triangle* triangles[4];

	Quadtree* children[4];

	Quadtree *      parent;

	Mesh* mesh;
//	Quadtree *      NW;
//	Quadtree *      NE;
//	Quadtree *      SW;
//	Quadtree *      SE;

//	bool            contains(Quadtree *child, Object *object);

//	bool            contains(float x, float y, float width, float height, Object *object);
};

#endif
