#pragma once

#include "hacks/EigenFixup.h"
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

	VectorType angularVelocityDelta;

	Array<SharedPointer<AttachedForce>> attachedForces;

	ChangeCallbackCalculateInverse<InertiaTensorType> inertiaTensor;

    PhysicsBody(const InertiaTensorType &parameterInertiaTensor) {
        inertiaTensor.set(parameterInertiaTensor);
    }

	const VectorType &getPosition() const;
	const VectorType &getLinearVelocity() const;

	// hard because it can do weird things to the physics engine, be careful!
	void setLinearVelocityHard(const VectorType &linearVelocity);
	void setPositionHard(const VectorType &position);
};
