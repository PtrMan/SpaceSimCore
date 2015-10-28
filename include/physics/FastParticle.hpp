#pragma once

#include <eigen/Dense>

#include "bindings/SharedPointer.h"
#include "physics/PhysicsBody.h"

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

    bool nextHitAnything; // flag for the next iteration if it did hit anything
    SharedPointer<PhysicsBody> nextHitBody; // next iteration: contains the body if it did hit anything
};
