#pragma once

#include <eigen/Dense>

struct AttachedForce {
	typedef Eigen::Matrix<double, 3, 1> VectorType;

	// TODO< can be float >
	VectorType objectLocalPosition; // position where the force is applied
	VectorType forceInNewton;
};
