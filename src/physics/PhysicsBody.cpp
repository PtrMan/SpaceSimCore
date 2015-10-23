#include "physics/PhysicsBody.h"

VectorType &PhysicsBody::getPosition() const {
	return rungeKuttaState.x;
}

VectorType &PhysicsBody::getLinearVelocity() const {
	return rungeKuttaState.v;
}
