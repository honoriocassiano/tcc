/*
 * Presets.cpp
 *
 *  Created on: 9 de jul de 2017
 *      Author: cassiano
 */

#include "Presets.h"

#include "Quadtree/Math/Randomizer.h"

namespace pssg {

std::vector<std::vector<Vector4<double>>>Presets::colors;

void Presets::Initialize() {
	// Mercury preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(Vector4<double>(0.80, 0.82, 0.80, 0.05));
	colors.back().push_back(Vector4<double>(0.75, 0.70, 0.75, 0.55));
	colors.back().push_back(Vector4<double>(0.85, 0.90, 0.95, 0.95));

	// Venus preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(135 / 255.0, 109 / 255.0, 60 / 255.0, 0.05));

	colors.back().push_back(
			Vector4<double>(192 / 255.0, 169 / 255.0, 128 / 255.0, 0.7));

	colors.back().push_back(
			Vector4<double>(207 / 255.0, 191 / 255.0, 155 / 255.0, 0.9));

	// Mars preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(Vector4<double>(0.50, 0.25, 0.25, 0.05));
	colors.back().push_back(Vector4<double>(0.95, 0.50, 0.40, 0.95));

	// Saturn preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(140 / 255.0, 124 / 255.0, 89 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(179 / 255.0, 128 / 255.0, 94 / 255.0, 0.35));
	colors.back().push_back(
			Vector4<double>(185 / 255.0, 167 / 255.0, 153 / 255.0, 0.67));
	colors.back().push_back(
			Vector4<double>(205 / 255.0, 173 / 255.0, 132 / 255.0, 0.85));
	colors.back().push_back(
			Vector4<double>(179 / 255.0, 124 / 255.0, 94 / 255.0, 0.95));

	// Jupyter
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(140 / 255.0, 124 / 255.0, 89 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(179 / 255.0, 128 / 255.0, 94 / 255.0, 0.35));
	colors.back().push_back(
			Vector4<double>(213 / 255.0, 101 / 255.0, 3 / 255.0, 0.35));
	colors.back().push_back(
			Vector4<double>(185 / 255.0, 167 / 255.0, 153 / 255.0, 0.67));
	colors.back().push_back(
			Vector4<double>(205 / 255.0, 173 / 255.0, 132 / 255.0, 0.85));
	colors.back().push_back(
			Vector4<double>(179 / 255.0, 124 / 255.0, 94 / 255.0, 0.95));

	// Uranus preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(48 / 255.0, 166 / 255.0, 196 / 255.0, 0.0));
	colors.back().push_back(
			Vector4<double>(167 / 255.0, 235 / 255.0, 244 / 255.0, 0.75));
	colors.back().push_back(
			Vector4<double>(19 / 255.0, 176 / 255.0, 193 / 255.0, 0.9));

	// Neptune preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(72 / 255.0, 113 / 255.0, 253 / 255.0, 0.0));
	colors.back().push_back(
			Vector4<double>(47 / 255.0, 64 / 255.0, 164 / 255.0, 0.6));
	colors.back().push_back(
			Vector4<double>(15 / 255.0, 119 / 255.0, 86 / 255.0, 0.73));
	colors.back().push_back(
			Vector4<double>(150 / 255.0, 254 / 255.0, 251 / 255.0, 0.95));

	// Earth preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(Vector4<double>(0.00, 0.00, 0.20, 0.05));
	colors.back().push_back(Vector4<double>(0.00, 0.00, 0.40, 0.35));
	colors.back().push_back(Vector4<double>(0.00, 0.00, 1.00, 0.45));
	colors.back().push_back(Vector4<double>(1.00, 1.00, 0.80, 0.52));
	colors.back().push_back(Vector4<double>(0.10, 0.50, 0.10, 0.55));
	colors.back().push_back(Vector4<double>(0.10, 0.40, 0.10, 0.65));
	colors.back().push_back(Vector4<double>(0.40, 0.40, 0.20, 0.70));
	colors.back().push_back(Vector4<double>(0.60, 0.70, 0.60, 0.85));
	colors.back().push_back(Vector4<double>(0.90, 0.95, 0.95, 0.92));
}

const std::vector<Vector4<double>>& Presets::GetRandomPreset() {
	return colors.at(Randomizer::GetInteger(0, colors.size() - 1));
}

const std::vector<Vector4<double> >& Presets::GetEarthPreset() {
	return colors.at(colors.size() - 1);
}

} /* namespace pssg */

