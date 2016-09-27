#ifndef MESH_H
#define MESH_H

#include "../vectors.h"
#include "../Array/array.h"
#include "../Array/bag.h"
#include "boundingbox.h"
#include "../argparser.h"

// TODO Verificar a necessidade dessa estrutura
#include <vector>

class Vertex;
class Edge;
class Triangle;
class VertexParent;

class CelestialBody;
class MeshDrawer;

// ======================================================================
// ======================================================================

class Mesh {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Mesh();
  virtual ~Mesh();
  void Load(const char *input_file);
    
  // ========
  // VERTICES
  int numVertices() const { return vertices->Count(); }
  Vertex* addVertex(const Vec3f &pos);

  void removeVertex(Vertex *vertex);
  // this creates a relationship between 3 vertices (2 parents, 1 child)
  void setParentsChild(Vertex *p1, Vertex *p2, Vertex *child);

  // Delete the relation between parents and child and return child
  Vertex* deleteParentsChildRelation(Vertex *p1, Vertex *p2);

  // this accessor will find a child vertex (if it exists) when given
  // two parent vertices
  Vertex* getChildVertex(Vertex *p1, Vertex *p2) const;
  // look up vertex by index from original .obj file
  Vertex* getVertex(int i) const {
    assert (i >= 0 && i < numVertices());
    Vertex *v = (*vertices)[i];
    assert (v != NULL);
    return v; }

  // =====
  // EDGES
  int numEdges() const { return edges->Count(); }
  // this efficiently looks for an edge with the given vertices, using a hash table
  Edge* getEdge(Vertex *a, Vertex *b) const;

  // =========
  // TRIANGLES
  int numTriangles() const { return triangles->Count(); }
  Triangle* addTriangle(Vertex *a, Vertex *b, Vertex *c);
  void removeTriangle(Triangle *t);

  // ===============
  // OTHER ACCESSORS
  BoundingBox* getBoundingBox() const { return bbox; }

  // ===============
  // OTHER FUNCTIONS
  void LoopSubdivision();
  void Simplification(int target_tri_count);

  std::vector<Triangle*> getTrianglesByVertex(Edge * e);

  void printTriangles(int limit = 1000);
  void printVertices(int limit = 1000);

  friend class CelestialBody;
  friend class MeshDrawer;

private:

  void computeFaceNormals();
  void computeVerticesNormals();

  // ==============
  // REPRESENTATION
  Array<Vertex*> *vertices;
  Bag<Edge*> *edges;
  Bag<Triangle*> *triangles;
  BoundingBox *bbox;
  Bag<VertexParent*> *vertex_parents;

};

// ======================================================================
// ======================================================================

#endif




