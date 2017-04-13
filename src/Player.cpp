/*
 * Player.cpp
 *
 *  Created on: 11 de abr de 2017
 *      Author: cassiano
 */

#include "Player.h"

Player::Player(const Vec3f& center, float _collisionRadius) :
		camera(
				new PerspectiveCamera(center, Vec3f() - center, Vec3f(0, 1, 0),
						20 * M_PI / 180.0)), collisionRadius(_collisionRadius) {

}

Player::~Player() {
	delete camera;
}

bool Player::isColliding(const Vec3f& point) const {
	return false;
}
