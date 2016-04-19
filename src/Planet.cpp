#include "Planet.h"
#include <stdio.h>

#include "vertex.h"
#include "vectors.h"

#define DEBUG_POINTS 0
#define DEBUG_INDEX 0
#define DEBUG_GLEW 0

GLfloat Planet::to_rad = M_PI / 180.0;

Planet::Planet(GLfloat radius, int horizontal_sections, int vertical_sections) :
		CelestialBody(Vec3f()) {
	this->radius = radius;

	this->h_sections = horizontal_sections;
	this->v_sections = vertical_sections;

	step_circle_angle = 360.0 / h_sections;
	step_height_angle = 180.0 / v_sections;

	this->vertex_size = h_sections * (v_sections + 1) * 3;
	this->index_size = 6 * h_sections * v_sections;

	this->vertex = new GLfloat[vertex_size];
	this->vertex_index = new GLuint[index_size];

	this->vertex_id = 0;
	this->vertex_index_id = 0;

	makePoints();
	makeIndexes();
}

Planet::~Planet() {
	delete[] vertex_index;
	delete[] vertex;
}

void Planet::show() {

	std::cout << "Drawing Sphere\n";

	makePoints();
	makeIndexes();
	makeVBO();

#if DEBUG_GLEW
	std::cout << glewGetErrorString(glGetError()) << std::endl;
#endif

#if DEBUG_POINTS
	for (int i = 0; i < vertex_size; i += 3) {
		std::cout << "(x, y, z) => (" << vertex[i] << ", " << vertex[i + 1]
		<< ", " << vertex[i + 2] << ")\n";
	}

	std::cout << "Number of Vertex: " << vertex_size / 3 << std::endl;
#endif

#if DEBUG_INDEX

	for (int i = 0; i < index_size; i += 3) {
		std::cout << "Triangle => (" << vertex_index[i] << ", "
		<< vertex_index[i + 1] << ", " << vertex_index[i + 2]
		<< ")\n";
	}

	std::cout << "Number of Index: " << index_size << std::endl;

#endif

//	perlin->show();
	update(0);
}

#define CIRCLE_NEXT_MOD(n, first) ( first +  ((n + 1) % h_sections) )

void Planet::makeIndexes() {

	int position = 0;

	int i_0 = 0, i_1 = 1, i_2 = h_sections, i_3 = h_sections, i_4 = 1, i_5 =
			h_sections + 1;

	for (int i = 1; i <= v_sections; ++i) {

		int first_circle_point = (i - 1) * h_sections;
		int first_next_circle_point = i * h_sections;

		int index_0 = i_0 + first_circle_point, index_1 = i_1
				+ first_circle_point, index_2 = i_2 + first_circle_point,
				index_3 = i_3 + first_circle_point, index_4 = i_4
						+ first_circle_point, index_5 = i_5
						+ first_circle_point;

		for (int j = 0; j < h_sections; ++j) {

//			vertex_index[position] = index_0;
//			vertex_index[position + 1] = index_1;
//			vertex_index[position + 2] = index_2;
//			vertex_index[position + 3] = index_3;
//			vertex_index[position + 4] = index_4;
//			vertex_index[position + 5] = index_5;

			mMesh->addTriangle(mMesh->getVertex(index_0),
					mMesh->getVertex(index_1), mMesh->getVertex(index_2));
			mMesh->addTriangle(mMesh->getVertex(index_3),
					mMesh->getVertex(index_4), mMesh->getVertex(index_5));

			index_0 = CIRCLE_NEXT_MOD(index_0, first_circle_point);
			index_1 = CIRCLE_NEXT_MOD(index_1, first_circle_point);
			index_2 = CIRCLE_NEXT_MOD(index_2, first_next_circle_point);

			index_3 = CIRCLE_NEXT_MOD(index_3, first_next_circle_point);
			index_4 = CIRCLE_NEXT_MOD(index_4, first_circle_point);
			index_5 = CIRCLE_NEXT_MOD(index_5, first_next_circle_point);

			position += 6;
		}
	}
}

void Planet::update(float time) {
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

//	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index_id);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

	glDrawElements(GL_TRIANGLES, index_size,
	GL_UNSIGNED_INT, (void *) 0);

	glDisableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//void Sphere::setCenter(const Vec3f& position) {
//	oldCenter = center;
//	center = position;
//
//	updatePoints();
//}

void Planet::updateVertexes() {
//	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), vertex,
//				 GL_STATIC_DRAW);
}

void Planet::makeVBO() {

//	glGenBuffers(1, &vertex_id);
//	glGenBuffers(1, &vertex_index_id);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index_id);
//
//	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), vertex,
//	GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(int),
//			vertex_index,
//			GL_STATIC_DRAW);

}

void Planet::makePoints() {

	int counter = 0;

	Perlin* perlin = Perlin::getInstance();

	for (int i = 0; i <= v_sections; ++i) {

		GLfloat height_angle = i * step_height_angle;

		GLfloat height_angle_rad = height_angle * to_rad;

		for (int j = 0; j < h_sections; ++j) {
			GLfloat circle_angle = j * step_circle_angle;

			GLfloat circle_angle_rad = circle_angle * to_rad;

			GLfloat x = radius * sin(height_angle_rad) * cos(circle_angle_rad);
			GLfloat z = radius * sin(height_angle_rad) * sin(circle_angle_rad);
			GLfloat y = radius * cos(height_angle_rad);

			float scale = 2.0;

			float p = 0.0;
			float a = 10.0, b = 2.0;

			float a_i = 1, b_i = 1;

			for (int octave = 0; octave < 6; ++octave) {
				p += perlin->generate( (b_i * (x + radius)) / a_i, (b_i * (y + radius)) / a_i,
						(b_i * (z + radius)) / a_i);

				a_i *= a;
				b_i *= b;
			}

			p *= 0.075;

			//std::cout << "(X, Y, Z) => (" << x/radius << ", "  << y/radius << ", "  << z/radius << ") " << "perlin: " << p << "\n";

			GLfloat p_x = x * p;
			GLfloat p_y = y * p;
			GLfloat p_z = z * p;

			mMesh->addVertex(Vec3f(x + p_x, y + p_y, z + p_z));

//			vertex[counter] = x + p_x;
//			vertex[counter + 1] = y + p_y;
//			vertex[counter + 2] = z + p_z;

			counter += 3;
		}
	}
}

void Planet::updatePoints() {
	for (int i = 0; i < mMesh->numVertices(); ++i) {
		//Vertex* v = mesh->getVertex(i);
		const Vec3f oldPosition = mMesh->getVertex(i)->get();

		Vec3f newPosition(0, 0, 0);

		newPosition = (oldPosition - oldCenter) + center;

//		newPosition.x = center.x + oldPosition.x;
//		newPosition.y = center.y + oldPosition.y;
//		newPosition.z = center.z + oldPosition.z;

		mMesh->getVertex(i)->set(newPosition);

		printf("(%f, %f, %f) => (%f, %f, %f)\n", oldPosition.x(),
				oldPosition.y(), oldPosition.z(), newPosition.x(),
				newPosition.y(), newPosition.z());
	}
}
