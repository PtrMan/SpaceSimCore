#include "physics/PhysicsBody.h"

VectorType &PhysicsBody::getPosition() const {
	return rungeKuttaState.x;
}

VectorType &PhysicsBody::getLinearVelocity() const {
	return rungeKuttaState.v;
}

void PhysicsBody::setLinearVelocityHard(const VectorType &linearVelocity) {
	rungeKuttaState.v = linearVelocity;
}

void PhysicsBody::setPositionHard(const VectorType &position) {
	rungeKuttaState.x = position;
}
