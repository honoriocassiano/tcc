/*
 * Constants.h
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include "Scale.h"

namespace constant {

inline double G() {
	return 6.67e-11 * Scale::getLength();
}

}

#endif /* SRC_CONSTANTS_H_ */
