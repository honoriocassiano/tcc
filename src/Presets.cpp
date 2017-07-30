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

	// Io preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(81 / 255.0, 81 / 255.0, 27 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(156 / 255.0, 122 / 255.0, 48 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(198 / 255.0, 192 / 255.0, 106 / 255.0, 0.45));
	colors.back().push_back(
			Vector4<double>(254 / 255.0, 244 / 255.0, 131 / 255.0, 0.8));
	colors.back().push_back(
			Vector4<double>(211 / 255.0, 133 / 255.0, 58 / 255.0, 0.98));

	// Europa preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(236 / 255.0, 236 / 255.0, 202 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(113 / 255.0, 67 / 255.0, 31 / 255.0, 0.8));

	// Ganymede preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(194 / 255.0, 180 / 255.0, 179 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(155 / 255.0, 132 / 255.0, 101 / 255.0, 0.45));
	colors.back().push_back(
			Vector4<double>(99 / 255.0, 72 / 255.0, 45 / 255.0, 0.8));

	// Callisto preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(28 / 255.0, 22 / 255.0, 24 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(61 / 255.0, 63 / 255.0, 49 / 255.0, 0.35));
	colors.back().push_back(
			Vector4<double>(118 / 255.0, 105 / 255.0, 86 / 255.0, 0.65));
	colors.back().push_back(
			Vector4<double>(146 / 255.0, 125 / 255.0, 96 / 255.0, 0.85));
	colors.back().push_back(
			Vector4<double>(220 / 255.0, 232 / 255.0, 212 / 255.0, 0.95));

	// Titan preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
			Vector4<double>(212 / 255.0, 161 / 255.0, 80 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(236 / 255.0, 194 / 255.0, 96 / 255.0, 0.35));
	colors.back().push_back(
			Vector4<double>(216 / 255.0, 196 / 255.0, 97 / 255.0, 0.95));

	// Enceladus preset
	colors.push_back(std::vector<Vector4<double>>());

	colors.back().push_back(
				Vector4<double>(55 / 255.0, 73 / 255.0, 80 / 255.0, 0.05));
	colors.back().push_back(
			Vector4<double>(189 / 255.0, 218 / 255.0, 231 / 255.0, 0.15));
	colors.back().push_back(
			Vector4<double>(166 / 255.0, 169 / 255.0, 174 / 255.0, 0.7));

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
//	return colors.at(colors.size() - 6);
//	return colors.at(1);
}

const std::vector<Vector4<double> >& Presets::GetEarthPreset() {
	return colors.at(colors.size() - 1);
}

} /* namespace pssg */

