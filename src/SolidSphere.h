#include <vector>
#include <GL/glut.h>
#include <cmath>

#ifndef SOLIDSPHERE_H
#define SOLIDSPHERE_H

class SolidSphere
{
    public:
        SolidSphere(float radius, unsigned int rings, unsigned int sectors);
        virtual ~SolidSphere();

        void draw(GLfloat x, GLfloat y, GLfloat z);

    protected:
        std::vector<GLfloat> sphere_vertices;
        std::vector<GLfloat> sphere_normals;
        std::vector<GLfloat> sphere_texcoords;
        std::vector<GLushort> sphere_indices;
};

#endif // SOLIDSPHERE_H
