/*
 * Player.h
 *
 *  Created on: 11 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include "structures/vectors.h"
#include "Collidable.h"
#include "PerspectiveCamera2.h"


class Player: public Collidable {
public:
	Player(const Vec3f& center, float collisionRadius = 5);
	virtual ~Player();

	bool isColliding(const Vec3f& point) const override;

private:
	PerspectiveCamera2* camera;
	float collisionRadius;

};

#endif /* SRC_PLAYER_H_ */
