/*
 * MathUtils.hpp
 *
 *  Created on: 17 de fev de 2017
 *      Author: aluno
 */

#ifndef SRC_MATHUTILS_HPP_
#define SRC_MATHUTILS_HPP_

namespace math {
float distanceFromPointToLine(const Vec3f& p1, const Vec3f& p2,
		const Vec3f& p) {

	auto temp1 = (p - p1);
	auto temp2 = (p - p2);

	Vec3f numerator, denominator = (p2 - p1);

	Vec3f::cross3(numerator, temp1, temp2);

	return numerator.length() / denominator.length();
}

}

#endif /* SRC_MATHUTILS_HPP_ */
