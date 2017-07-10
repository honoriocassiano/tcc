/*
 * Presets.h
 *
 *  Created on: 9 de jul de 2017
 *      Author: cassiano
 */

#ifndef SRC_PRESETS_H_
#define SRC_PRESETS_H_

#include <vector>
#include "Quadtree/Math/VectorMath.h"

namespace pssg {

class Presets {
public:
//	Presets();
//	virtual ~Presets();
	static void Initialize();

	static const std::vector<Vector4<double>>& GetRandomPreset();
	static const std::vector<Vector4<double>>& GetEarthPreset();

private:
	static std::vector<std::vector<Vector4<double>>> colors;
};

} /* namespace pssg */

#endif /* SRC_PRESETS_H_ */
