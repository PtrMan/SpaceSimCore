#pragma once

#include <eigen/Dense>
#include <eigen/Geometry>

#include "ChangeCallbackCalculateInverse.hpp"

// https://en.wikiversity.org/wiki/Physics_equations/Impulse,_momentum,_and_motion_about_a_fixed_axis
// r is the distance from the orgin of the body where the force is applied
template<typename ScalarType>
Eigen::Matrix<ScalarType, 3, 1> calculateTorque(const Eigen::Matrix<ScalarType, 3, 1> &r, const Eigen::Matrix<ScalarType, 3, 1> &force) {
	return r.cross(force);
}

// calculate the angular acceleration of a torque (which is calculated with calculate torque)
// used to calculate the angular acceleration of a body when a force pulls on it

// derivation of formula:
// https://en.wikiversity.org/wiki/Physics_equations/Impulse,_momentum,_and_motion_about_a_fixed_axis
// tau = I * alpha  | * I^(-1)
// alpha = tau * (I^(-1))
template<typename VectorType, typename ScalarType>
VectorType calculateRotationalAcceleration(ChangeCallbackCalculateInverse<Eigen::Matrix<ScalarType, 3, 3>> &inertiaTensorAndInverse, const VectorType &torque) {
	return inertiaTensorAndInverse.getInverse() * torque;
}
