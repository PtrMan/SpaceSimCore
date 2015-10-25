#pragma once

#include "physics/PhysicsBody.h"
#include "physics/CelestialBody.h"
#include "physics/FastParticle.hpp"

#include "math/RungeKutta4.h"

#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

#include "hacks/EigenFixup.h"
#include <eigen/Dense>

class PhysicsEngine {
    typedef Eigen::Matrix<float, 3, 1> VectorSingleType;
    typedef Eigen::Matrix<double, 3, 3> InertiaTensorType;
    typedef ChangeCallbackCalculateInverse<InertiaTensorType> InertialChangeCallbackType;


	class AccelerationImplementation : public IAcceleration<Eigen::Matrix<double, 3, 1>> {
	public:
		void setPhysicsEngine(PhysicsEngine *physicsEngine);

		virtual Eigen::Matrix<double, 3, 1> calculateAcceleration(const RungeKutta4State<Eigen::Matrix<double, 3, 1>> &state, const float time) const;

		void setCurrentBodyMass(const double currentBodyMass);

		PhysicsEngine *physicsEngine;
		double currentBodyMass;
	};

public:
	PhysicsEngine();

	void completeStep(const float timeDelta);

	void step(const float timeDelta);

	// transfers/calculates the new state
	void postStep(const float timeDelta);

	const Array<SharedPointer<CelestialBody>> &getCelestialBodies() const;

	void addPhysicsBody(SharedPointer<PhysicsBody> physicsBody);
	void removePhysicsBody(SharedPointer<PhysicsBody> physicsBody);

	void addCelestialBody(SharedPointer<CelestialBody> celestialBody);
	void removeCelestialBody(SharedPointer<CelestialBody> celestialBody);

	void addFastParticle(SharedPointer<FastParticle> particle);
	void removeFastParticle(SharedPointer<FastParticle> particle);
protected:
    // NOT IMPLEMENTED
    // see https://en.wikipedia.org/wiki/Collision_response#Impulse-Based_Reaction_Model
    static float calcReactionImpulseMagnitude(
            const InertialChangeCallbackType &inertia1,
            float mass1,
            const VectorSingleType &r1,

            const InertialChangeCallbackType &inertia2,
            float mass2,
            const VectorSingleType &r2,

            const VectorSingleType &n,
            const float coefficientOfRestitution
    );



	Array<SharedPointer<PhysicsBody>> physicsBodies;
	Array<SharedPointer<CelestialBody>> celestialBodies;
	Array<SharedPointer<FastParticle>> fastParticles;

	RungeKutta4<Eigen::Matrix<double, 3, 1>> rungeKutta4;
};

