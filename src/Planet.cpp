#include "Planet.h"
#include <stdio.h>
#include <vector>

#include "vertex.h"
#include "vectors.h"

#include "triangle.h"

#define DEBUG_POINTS 0
#define DEBUG_INDEX 0
#define DEBUG_GLEW 0

#define X .525731112119133606
#define Z .850650808352039932

const GLfloat Planet::vdata[12][3] = { { -X, 0.0, Z }, { X, 0.0, Z }, { -X, 0.0,
		-Z }, { X, 0.0, -Z }, { 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, {
		0.0, -Z, -X }, { Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X,
		0.0 } };
const GLuint Planet::tindices[20][3] = { { 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 },
		{ 4, 5, 8 }, { 4, 8, 1 }, { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5,
				2, 3 }, { 2, 7, 3 }, { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, {
				11, 0, 6 }, { 0, 1, 6 }, { 6, 1, 10 }, { 9, 0, 11 },
		{ 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 } };

int Planet::octaves = 8;
int Planet::A = 10;
int Planet::B = 2;

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

	mTurbulenceDistortion = 0.1f;

	makePoints();
	makeTriangles();
//	makePoints();
//	makeIndexes();

	for (int i = 0; i < 6; ++i) {
		subdivide();
	}
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

//#define CIRCLE_NEXT_MOD(n, first) ( first +  ((n + 1) % h_sections) )
#define CIRCLE_NEXT_MOD(n, first) ( first + ((n + 1) % h_sections) )

void Planet::makeIndexes() {

	int position = 0;

//	int i_0 = 0;
//	int i_1 = 1;
//	int i_2 = h_sections;
//	int i_3 = h_sections;
//	int i_4 = 1;
//	int i_5 = h_sections + 1;
	int i_0 = 1;
	int i_1 = 2;
	int i_2 = h_sections + 1;
	int i_3 = h_sections + 1;
	int i_4 = 2;
	int i_5 = h_sections + 2;

	for (int i = 1; i <= v_sections - 1; ++i) {

//		int first_circle_point = (i - 1) * h_sections;
//		int first_next_circle_point = i * h_sections;
		int first_circle_point = (i - 1) * h_sections + 1;
		int first_next_circle_point = i * h_sections + 1;

//		int index_0 = i_0 + first_circle_point;
//		int index_1 = i_1 + first_circle_point;
//		int index_2 = i_2 + first_circle_point;
//		int index_3 = i_3 + first_circle_point;
//		int index_4 = i_4 + first_circle_point;
//		int index_5 = i_5 + first_circle_point;
		int index_0 = i_0 + first_circle_point - 1;
		int index_1 = i_1 + first_circle_point - 1;
		int index_2 = i_2 + first_circle_point - 1;
		int index_3 = i_3 + first_circle_point - 1;
		int index_4 = i_4 + first_circle_point - 1;
		int index_5 = i_5 + first_circle_point - 1;

		for (int j = 0; j < h_sections; ++j) {

//			vertex_index[position] = index_0;
//			vertex_index[position + 1] = index_1;
//			vertex_index[position + 2] = index_2;
//			vertex_index[position + 3] = index_3;
//			vertex_index[position + 4] = index_4;
//			vertex_index[position + 5] = index_5;

			if (i == 1 || i == v_sections) {
				printf("%d = (%d, %d)\n", i - 1, j + first_circle_point,
						j + first_circle_point + 1);

				int current = (j + first_circle_point) % h_sections;
				current = (current == 0) ? h_sections : current;

				int next = (j + first_circle_point + 1) % h_sections;
				next = (next == 0) ? h_sections : next;

				mMesh->addTriangle(mMesh->getVertex(i - 1),
						mMesh->getVertex(current), mMesh->getVertex(next)); //(j + first_circle_point + 1) % h_sections));

//				index_0 = CIRCLE_NEXT_MOD(index_0, first_circle_point);
//				index_1 = CIRCLE_NEXT_MOD(index_1, first_circle_point);
			} else {
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
	for (int i = 0; i < 12; ++i) {

		float x = vdata[i][0];
		float y = vdata[i][1];
		float z = vdata[i][2];

		Vec3f position(x, y, z);

		//float turbulence = Perlin::generateTurbulence(octaves, A, B, x + 1, y + 1, z + 1);
		float turbulence = Perlin::generateTurbulence(octaves, A, B,
				(x + 1) * radius, (y + 1) * radius, (z + 1) * radius);

		position += (position * turbulence * mTurbulenceDistortion);

		mMesh->addVertex(position);

//		float scale = 0.25;
//
//					float p = 0.0;
//					float a = 10.0, b = 2.0;
//
//					float a_i = 1, b_i = 1;
//
//					for (int octave = 0; octave < 6; ++octave) {
//						p += perlin->generate((b_i * (x + radius) * scale) / a_i,
//								(b_i * (y + radius) * scale) / a_i,
//								(b_i * (z + radius) * scale) / a_i);
//
//						a_i *= a;
//						b_i *= b;
//					}
//
//					p *= 0.075;
//
//					//std::cout << "(X, Y, Z) => (" << x/radius << ", "  << y/radius << ", "  << z/radius << ") " << "perlin: " << p << "\n";
//
//					GLfloat p_x = x * p;
//					GLfloat p_y = y * p;
//					GLfloat p_z = z * p;
	}
}

/*
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

 float scale = 0.25;

 float p = 0.0;
 float a = 10.0, b = 2.0;

 float a_i = 1, b_i = 1;

 for (int octave = 0; octave < 6; ++octave) {
 p += perlin->generate((b_i * (x + radius) * scale) / a_i,
 (b_i * (y + radius) * scale) / a_i,
 (b_i * (z + radius) * scale) / a_i);

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

 if (i == 0 || i == v_sections) {
 break;
 }
 }
 }
 }
 */

void Planet::subdivide() {
//	void subdivide(float *v1, float *v2, float *v3)
//	{
//	   GLfloat v12[3], v23[3], v31[3];
//	   GLint i;
//
//	   for (i = 0; i < 3; i++) {
//	      v12[i] = v1[i]+v2[i];
//	      v23[i] = v2[i]+v3[i];
//	      v31[i] = v3[i]+v1[i];
//	   }
//	   normalize(v12);
//	   normalize(v23);
//	   normalize(v31);
//	   drawtriangle(v1, v12, v31);
//	   drawtriangle(v2, v23, v12);
//	   drawtriangle(v3, v31, v23);
//	   drawtriangle(v12, v23, v31);
//	}

	int count = mMesh->numTriangles();

	Bag<Triangle*>* triangles = getTriangles();

	auto iterator = triangles->StartIteration();

	std::vector<Triangle*> ts;

	while (Triangle* triangle = iterator->GetNext()) {
		ts.push_back(triangle);
	}
	triangles->EndIteration(iterator);

	Vertex * v01 = nullptr;
	Vertex * v12 = nullptr;
	Vertex * v20 = nullptr;

	for (int i = 0; i < count; ++i) {
		Triangle * t = ts[i]; //iterator->GetNext();

		Vertex * v0 = (*t)[0];
		Vertex * v1 = (*t)[1];
		Vertex * v2 = (*t)[2];

		Vec3f p01 = v0->get() + v1->get();
		Vec3f p12 = v1->get() + v2->get();
		Vec3f p20 = v2->get() + v0->get();

		p01.Normalize();
		p12.Normalize();
		p20.Normalize();

		float turbulence01 = Perlin::generateTurbulence(octaves, A, B,
				(p01.x() + 1) * radius, (p01.y() + 1) * radius, (p01.z() + 1) * radius);
		float turbulence12 = Perlin::generateTurbulence(octaves, A, B,
				(p12.x() + 1) * radius, (p12.y() + 1) * radius, (p12.z() + 1) * radius);
		float turbulence20 = Perlin::generateTurbulence(octaves, A, B,
				(p20.x() + 1) * radius, (p20.y() + 1) * radius, (p20.z() + 1) * radius);

		p01 += p01 * turbulence01 * mTurbulenceDistortion;
		p12 += p12 * turbulence12 * mTurbulenceDistortion;
		p20 += p20 * turbulence20 * mTurbulenceDistortion;
		//position *= turbulence * 0.05f;

		v01 = mMesh->addVertex(p01);
		v12 = mMesh->addVertex(p12);
		v20 = mMesh->addVertex(p20);

		mMesh->removeTriangle(t);
		//ts.push_back(t);

		//printf("(%d, %d, %d)\n", v0->getIndex(), v1->getIndex(), v2->getIndex());

		mMesh->addTriangle(v0, v01, v20);
		mMesh->addTriangle(v1, v12, v01);
		mMesh->addTriangle(v2, v20, v12);
		mMesh->addTriangle(v01, v12, v20);

		//	   drawtriangle(v1, v12, v31);

		//	   drawtriangle(v2, v23, v12);

		//	   drawtriangle(v3, v31, v23);

		//	   drawtriangle(v12, v23, v31);
	}
}

void Planet::makeTriangles() {
	for (int i = 0; i < 20; ++i) {
		Vertex * v0 = mMesh->getVertex(tindices[i][0]);
		Vertex * v1 = mMesh->getVertex(tindices[i][1]);
		Vertex * v2 = mMesh->getVertex(tindices[i][2]);

		mMesh->addTriangle(v0, v2, v1);
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
