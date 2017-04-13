#include "Debug.h"
#include <assert.h>

#include <GL/glut.h>
#include "structures/vectors.h"
#include "PriorityQueue.h"
#include "Planet.h"

#include "SDLWindow.h"

#include "Quadtree.h"
#include "QuadtreeMesh.h"

#include "DirectionArray.h"
#include "IntercardinalDirection.h"

#include "MultiLevelArray.h"

#include "Greeting.h"


// =========================================
// =========================================

int main(int argc, char **argv) {

	sayHello();

	SDLWindow window(800, 600);
	window.run();


//	auto abc = MultiLevelArray<int>(4);
//
////	for (int i = 0; i < 10; ++i) {
////		for (int j = 0; j < 5; ++j) {
////			abc.add(i + j * 10, j);
////		}
////	}
//
//	abc.add(1, 1);
//
//	abc.add(3, 3);
//
//	for (auto it = abc.rbegin(2); it != abc.rend(); ++it) {
//
////		printf("{ %ld, %ld } -> { %ld, %ld }\n", it.getPosition().first,
////				it.getPosition().second, abc.rend().getPosition().first,
////				abc.rend().getPosition().second);
//
//		const auto& n = *it;
//
//		printf("%d\n", n);
//	}
//	printf("\n");

//
//	srand(time(nullptr));
//
//	for (int i = 0; i < 10; ++i) {
//		auto rLevel = rand() % 5;
//
//		abc.remove(std::make_pair(rLevel, 0));
//	}
//
//	for (const auto& el : abc) {
//		printf("%d ", el);
//	}
//	printf("\n");
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

	Log("Finished!\n");

	return 0;
}
