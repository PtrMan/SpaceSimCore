#include "physics/FastParticle.hpp"
#include "collision/RaySphereRelativeStructureOfArrays.hpp"
#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

/**
 * Encapsulates the whole datatransfer and collision evaluation calculations for the efficient ray sphere tests
 *
 */
namespace SphereRayHelper {
	static const unsigned batchSize = 16;

	void transferFastParticlesToRaySphereSoa(const Array<SharedPointer<FastParticle>> &fastParticles, const float collisionSphereRadius, const Eigen::Matrix<double, 3, 1> &collisionSpherePosition, RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays);

	void doBatchedCollisionTests(RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays);

	// - check for positive intersect, if true, check if plus or minus is in the length of the ray
	void checkForCollisions(const RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays, const Array<SharedPointer<FastParticle>> &fastParticles, Array<int> &collisionTestResult);
}
