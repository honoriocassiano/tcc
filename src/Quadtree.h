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
enum Intercardinal
	: int {
		NW = 0, NE, SW, SE
};

enum Cardinal
	: int {
		N = 0, E, S, W
};
}

typedef struct _qtd {
	const directions::Intercardinal data[2];
	Quadtree * tree;

	enum Side {
		LEFT = 0, RIGHT
	};

	_qtd(directions::Intercardinal l, directions::Intercardinal r, Quadtree * tree) :
			data { l, r }, tree(tree) {

	}

} QuadtreeData;

//typedef struct _qtd {
//	Quadtree * data[2] = { nullptr, nullptr };
//
//	enum SIDE {
//		LEFT = 0, RIGHT
//	};
//
//	_qtd(Quadtree * l, Quadtree * r) :
//			data { l, r } {
//
//	}
//
//} QuadtreeData;

class Quadtree {
public:
	Quadtree(const Vec3f& origin, const Vec3f& xAxis, const Vec3f& yAxis,
			int level, int maxLevel);

	~Quadtree();

	void subdivide(directions::Intercardinal point, bool propagate = true);

	void merge(directions::Intercardinal point);

	inline Mesh* getMesh() {
		return mesh;
	}

	inline Quadtree* getChild(directions::Intercardinal direction) {
		return children[direction];
	}

	void setNeighbourAt(directions::Cardinal position, Quadtree* value);
	void setNeighbours(Quadtree* n, Quadtree* e, Quadtree* s, Quadtree* w);

	inline int getLevel() const {
		return level;
	}

protected:

	Quadtree(Vertex* nw, Vertex* ne, Vertex* sw, Vertex* se, int level,
			int maxLevel, Mesh * mesh, Quadtree* parent);

	Vertex** getVertices() {
		return vertices;
	}

	int findPosition(Triangle* triangle) const;

	friend class QuadTree;

private:

	Vec3f origin;
	Vec3f xAxis;
	Vec3f yAxis;

	int level;
	int maxLevel;

	Vertex* middle;

	Vertex* vertices[4];
	Triangle* triangles[4];

	Quadtree* children[4];

	Quadtree* neighbours[4];

	Quadtree * parent;

	Mesh* mesh;
};

#endif
