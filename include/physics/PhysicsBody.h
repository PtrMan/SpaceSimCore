#pragma once

#include <eigen/Dense>

#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

#include "math/RungeKutta4.h"

#include "ChangeCallbackCalculateInverse.hpp"

#include "physics/AttachedForce.h"

struct PhysicsBody {
	typedef Eigen::Matrix<double, 3, 1> VectorType;
	typedef Eigen::Matrix<double, 3, 3> InertiaTensorType;

	double mass;

	RungeKutta4State<VectorType> rungeKuttaState;
	VectorType angular; // TODO< use quaternoins? >
	VectorType angularVelocity;

	Array<SharedPointer<AttachedForce>> attachedForces;

	// TODO< make work>
	ChangeCallbackCalculateInverse<InertiaTensorType> inertiaTensor;

	PhysicsBody(const InertiaTensorType &parameterInertiaTensor) : inertiaTensor(parameterInertiaTensor) {};

	// TODO< accessors for position and velocity >
};