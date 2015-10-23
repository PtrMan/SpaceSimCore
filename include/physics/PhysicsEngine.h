#include "physics/PhysicsBody.h"
#include "physics/CelestialBody.h"

#include "math/RungeKutta4.h"

#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

#include "hacks/EigenFixup.h"
#include <eigen/Dense>

class PhysicsEngine {
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

	void step(const float timeDelta);

	const Array<SharedPointer<CelestialBody>> &getCelestialBodies() const;

	void addPhysicsBody(SharedPointer<PhysicsBody> physicsBody);
	void removePhysicsBody(SharedPointer<PhysicsBody> physicsBody);

	void addCelestialBody(SharedPointer<CelestialBody> celestialBody);
	void removeCelestialBody(SharedPointer<CelestialBody> celestialBody);


protected:
	Array<SharedPointer<PhysicsBody>> physicsBodies;
	Array<SharedPointer<CelestialBody>> celestialBodies;

	RungeKutta4<Eigen::Matrix<double, 3, 1>> rungeKutta4;
};

