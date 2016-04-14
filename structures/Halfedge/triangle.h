#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <limits.h>
#include "boundingbox.h"
#include "edge.h"

// ===========================================================

class Triangle {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Triangle() {
    edge = NULL; }
  ~Triangle() {}

  // here's the hash function to use for triangles so they
  // can be efficiently accessed within the Bag data structure
  static void extract_func(Triangle *t, int &a, int &b, int &c) {
    a = (*t)[0]->getIndex(); 
    b = (*t)[1]->getIndex(); 
    c = (*t)[2]->getIndex();
  }

  // =========
  // ACCESSORS
  Vertex* operator[](int i) const { 
    assert (edge != NULL);
    if (i==0) return edge->getVertex();
    if (i==1) return edge->getNext()->getNext()->getVertex();
    if (i==2) return edge->getNext()->getVertex();
    assert(0);
  }
  Edge* getEdge() { 
    assert (edge != NULL);
    return edge; 
  }
  void setEdge(Edge *e) {
    assert (edge == NULL);
    edge = e;
  }

  // NOTE: If you want to modify a triangle, remove it from the mesh,
  // delete it, create a new copy with the changes, and re-add it.
  // This will ensure the edges get updated appropriately.

protected:

  // don't use this constructor
  Triangle& operator = (const Triangle &t) { assert(0); }
  
  // ==============
  // REPRESENTATION
  Edge *edge;

};

// ===========================================================

#endif
