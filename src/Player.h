/*
 * Player.h
 *
 *  Created on: 11 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include "PerspectiveCamera.h"
#include "structures/vectors.h"
#include "Collidable.h"


class Player: public Collidable {
public:
	Player(const Vec3f& center, float collisionRadius = 5);
	virtual ~Player();

	bool isColliding(const Vec3f& point) const override;

private:
	PerspectiveCamera* camera;
	float collisionRadius;

};

#endif /* SRC_PLAYER_H_ */
