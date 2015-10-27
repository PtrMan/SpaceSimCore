#include "physics/PhysicsEngine.h"

#include "physics/Angular.hpp"

#include "collision/SphereRayHelper.hpp"

void PhysicsEngine::AccelerationImplementation::setPhysicsEngine(PhysicsEngine *physicsEngine) {
	this->physicsEngine = physicsEngine;
}

Eigen::Matrix<double, 3, 1> PhysicsEngine::AccelerationImplementation::calculateAcceleration(const RungeKutta4State<Eigen::Matrix<double, 3, 1>> &state, const float time) const {
	Eigen::Matrix<double, 3, 1> temporaryForce = Eigen::Matrix<double, 3, 1>(0.0, 0.0, 0.0);

	for( int index = 0; index < physicsEngine->getCelestialBodies().getCount(); index++ ) {
		// TODO< calculate squared distance and calculate inverse force, add it to the result force
	}

    const Eigen::Matrix<double, 3, 1> acceleration = temporaryForce / currentBodyMass;
	return acceleration;
}

void PhysicsEngine::AccelerationImplementation::setCurrentBodyMass(const double currentBodyMass) {
	this->currentBodyMass = currentBodyMass;
}

PhysicsEngine::PhysicsEngine() {
	rungeKutta4.acceleration = new AccelerationImplementation();
	static_cast<AccelerationImplementation*>(rungeKutta4.acceleration)->setPhysicsEngine(this);
}

void PhysicsEngine::completeStep(const float timeDelta) {
	step(timeDelta);
	postStep(timeDelta);
}

void PhysicsEngine::step(const float timeDelta) {
    calcForcesAndReactionsForRigidBodies(timeDelta);
	collisionTestFastParticles();
}

void PhysicsEngine::postStep(const float timeDelta) {
	for( int index = 0; index < physicsBodies.getCount(); index++ ) {
		SharedPointer<PhysicsBody> currentBody = physicsBodies[index];

		currentBody->angularVelocity += currentBody->angularVelocityDelta;

		currentBody->angular += (currentBody->angularVelocity * timeDelta);

		currentBody->rungeKuttaState.update();
	}

	for( int index = 0; index < fastParticles.getCount(); index++ ) {
		SharedPointer<FastParticle> currentParticle = fastParticles[index];

		currentParticle->previousPosition = currentParticle->currentPosition;
		currentParticle->currentPosition = currentParticle->nextCurrentPosition;
	}
}

const Array<SharedPointer<CelestialBody>> &PhysicsEngine::getCelestialBodies() const {
	return celestialBodies;
}

void PhysicsEngine::collisionTestFastParticles() {
    raySphereRelative.resizeToSizeIfRequiredWithBatchSize(fastParticles.getCount(), SphereRayHelper::batchSize);

    Array<int> collisionTestResult;
    collisionTestResult.setNumberOfElements(fastParticles.getCount(), false);


    for( int index = 0; index < physicsBodies.getCount(); index++ ) {
        SharedPointer<PhysicsBody> currentBody = physicsBodies[index];

        // TODO< radius for body >
        float collisionSphereRadius = 0.0f;

        SphereRayHelper::transferFastParticlesToRaySphereSoa(fastParticles, collisionSphereRadius, currentBody->getPosition(), raySphereRelative);
        SphereRayHelper::doBatchedCollisionTests(raySphereRelative);
        SphereRayHelper::checkForCollisions(raySphereRelative, fastParticles, collisionTestResult);

        // TODO< process the collision test result >
    }
}

void PhysicsEngine::calcForcesAndReactionsForRigidBodies(float timeDelta) {
    for( int index = 0; index < physicsBodies.getCount(); index++ ) {
        SharedPointer<PhysicsBody> currentBody = physicsBodies[index];

        currentBody->angularVelocityDelta = Eigen::Matrix<double, 3, 1>(0.0, 0.0, 0.0);

        // TODO< put into method >

        for( int attachedForceIterator = 0; attachedForceIterator < currentBody->attachedForces.getCount(); attachedForceIterator++ ) {
            SharedPointer<AttachedForce> currentAttachedForce = currentBody->attachedForces[attachedForceIterator];

            // TODO< split each force into linear and angular components >

            Eigen::Matrix<double, 3, 1> angularComponentForce = currentAttachedForce->forceInNewton;

            Eigen::Matrix<double, 3, 1> appliedTorque = calculateTorque(currentAttachedForce->objectLocalPosition, angularComponentForce);
            Eigen::Matrix<double, 3, 1> rotationalAcceleration = calculateRotationalAcceleration(currentBody->inertiaTensor, appliedTorque);

            currentBody->angularVelocityDelta += (rotationalAcceleration * timeDelta);
        }


        static_cast<AccelerationImplementation*>(rungeKutta4.acceleration)->setCurrentBodyMass(currentBody->mass);
        rungeKutta4.integrate(currentBody->rungeKuttaState, 0.0, static_cast<double>(timeDelta));


    }

    // TODO< put into own method >
    for( int index = 0; index < fastParticles.getCount(); index++ ) {
        SharedPointer<FastParticle> currentParticle = fastParticles[index];

        currentParticle->nextCurrentPosition = currentParticle->currentPosition + (currentParticle->velocity * timeDelta);
    }
}

void PhysicsEngine::addPhysicsBody(SharedPointer<PhysicsBody> physicsBody) {
	physicsBodies.add(physicsBody);
}

void PhysicsEngine::removePhysicsBody(SharedPointer<PhysicsBody> physicsBody) {
	physicsBodies.remove(physicsBody);
}

void PhysicsEngine::addCelestialBody(SharedPointer<CelestialBody> celestialBody) {
	celestialBodies.add(celestialBody);
}

void PhysicsEngine::removeCelestialBody(SharedPointer<CelestialBody> celestialBody) {
	celestialBodies.remove(celestialBody);
}

void PhysicsEngine::addFastParticle(SharedPointer<FastParticle> particle) {
	fastParticles.add(particle);
}

void PhysicsEngine::removeFastParticle(SharedPointer<FastParticle> particle) {
	fastParticles.remove(particle);
}
