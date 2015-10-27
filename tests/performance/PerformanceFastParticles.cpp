#include "physics/PhysicsEngine.h"
#include "physics/InertiaHelper.hpp"

int main() {
	PhysicsEngine physicsEngine;

	for(int bodyI = 0; bodyI < 1; bodyI++ ) {
		SharedPointer<PhysicsBody> createdBody = SharedPointer<PhysicsBody>(new PhysicsBody(InertiaHelper::calcInertiaTensorForSolidSphere(1.0, 1.0)));

		physicsEngine.addPhysicsBody(createdBody);
	}

	for(int fastParticleI = 0; fastParticleI < 100000; fastParticleI++) {
		SharedPointer<FastParticle> createdFastParticle = SharedPointer<FastParticle>(new FastParticle());

		createdFastParticle->currentPosition = Eigen::Matrix<double, 3, 1>(100.0, 0.0, 0.0);
		createdFastParticle->previousPosition = Eigen::Matrix<double, 3, 1>(10.0, 0.0, 0.0);
		createdFastParticle->velocity = Eigen::Matrix<double, 3, 1>(10.0, 0.0, 0.0);

		physicsEngine.addFastParticle(createdFastParticle);
	}

	for( int iteration = 0; iteration < 1000; iteration++ ) {
		physicsEngine.step(0.1f);
	}


	
}