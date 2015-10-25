#pragma once

#include <eigen/Dense>

/**
 * Particle which moves too fast for a simple distance check
 * such as Bullets or not lightspeed fast rays
 *
 */
struct FastParticle {
	typedef Eigen::Matrix<double, 3, 1> VectorType;

	VectorType currentPosition;
	VectorType previousPosition;

	VectorType nextCurrentPosition; // for double bufering like physics

	VectorType velocity;
};
