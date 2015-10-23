#include "physics/PhysicsBody.h"

const PhysicsBody::VectorType &PhysicsBody::getPosition() const {
	return rungeKuttaState.x;
}

const PhysicsBody::VectorType &PhysicsBody::getLinearVelocity() const {
	return rungeKuttaState.v;
}

void PhysicsBody::setLinearVelocityHard(const PhysicsBody::VectorType &linearVelocity) {
	rungeKuttaState.v = linearVelocity;
}

void PhysicsBody::setPositionHard(const PhysicsBody::VectorType &position) {
	rungeKuttaState.x = position;
}


