/*******************************************************************************
* File:           QuadtreeTerrain.h
* Author:         Christian Alfons
* Date created:   2010-06-31
* Date modified:  2011-02-13
* Description:    Quadtree chunked level-of-detail terrain class. Each instance
*                 has a number of faces, which in turn hold quadtree nodes.
*                 This class also holds data shared by the faces, such as
*                 camera position and view frustum.
*                 Perhaps most importantly, this class is used to communicate
*                 terrain data outside the quadtree structure.
*******************************************************************************/

#ifndef QUADTREETERRAIN_H_INCLUDED
#define QUADTREETERRAIN_H_INCLUDED

#include "../Geometry.h"
#include "QuadtreeTerrainFace.h"

class QuadtreeTerrain
{
  protected:
    // Instance variables
    vector<QuadtreeTerrainFace*>  faces;
    Vector3<double>               cameraPosition;
    Frustum                       frustum;

  public:
    // Constructor
    QuadtreeTerrain();

    // Destructor
    virtual ~QuadtreeTerrain();

    // Instance methods
    virtual void            Rebuild();
    Vector3<double>         GetCameraPositionLocal() const;
    void                    SetCameraPosition(const Vector3<double> &cameraPosition);
    const Frustum&          GetFrustum() const;
    void                    SetFrustum(const Frustum &frustum);
    virtual double          GetTerrainHeight(const Vector3<double> &point) const = 0;
    virtual Vector3<double> ProjectOnSurface(const Vector3<double> &point) const = 0;
    virtual void            Render() const;
    virtual void            Update();
};

#endif
