/*
 * QuadCube.h
 *
 *  Created on: 9 de set de 2016
 *      Author: aluno
 */

#ifndef SRC_QUADCUBE_H_
#define SRC_QUADCUBE_H_

class QuadCube {
public:
	QuadCube();
	virtual ~QuadCube();

	Face* faces[6];
};

#endif /* SRC_QUADCUBE_H_ */
