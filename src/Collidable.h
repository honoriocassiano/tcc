/*
 * Collidable.h
 *
 *  Created on: 11 de abr de 2017
 *      Author: cassiano
 */

#ifndef SRC_COLLIDABLE_H_
#define SRC_COLLIDABLE_H_

class Vec3f;

class Collidable {
public:

	virtual ~Collidable() {
	}

	virtual bool isColliding(const Vec3f& point) const = 0;

};

#endif /* SRC_COLLIDABLE_H_ */
