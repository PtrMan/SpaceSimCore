#include "physics/FastParticle.hpp"
#include "collision/RaySphereRelativeStructureOfArrays.hpp"
#include "math/RaySphere.hpp"
#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

/**
 * Encapsulates the whole datatransfer and collision evaluation calculations for the efficient ray sphere tests
 *
 */
namespace SphereRayHelper {
	static const unsigned batchSize = 16;

	void transferFastParticlesToRaySphereSoa(const Array<SharedPointer<FastParticle>> &fastParticles, const float collisionSphereRadius, const Eigen::Matrix<double, 3, 1> &collisionSpherePosition, RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays) {
	
		raySphereStructureOfArrays.resizeToSizeIfRequiredWithBatchSize(fastParticles.getCount(), batchSize);

		// calculate difference between ray positions and sphere position
		for( int index = 0; index < fastParticles.getCount(); index++ ) {
			SharedPointer<FastParticle> currentParticle = fastParticles[index];

			const Eigen::Matrix<double, 3, 1> positionDifference = currentParticle->currentPosition - collisionSpherePosition;
			reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionX.getAlignedPtr())[index] = static_cast<float>(positionDifference(0));
			reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionY.getAlignedPtr())[index] = static_cast<float>(positionDifference(1));
			reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionZ.getAlignedPtr())[index] = static_cast<float>(positionDifference(2));
		}

		// calculate (normalized) directions of spheres
		for( int index = 0; index < fastParticles.getCount(); index++ ) {
			SharedPointer<FastParticle> currentParticle = fastParticles[index];

			const Eigen::Matrix<double, 3, 1> normalizedVelocity = currentParticle->velocity.normalized();
			reinterpret_cast<float*>(raySphereStructureOfArrays.directionX.getAlignedPtr())[index] = static_cast<float>(normalizedVelocity(0));
			reinterpret_cast<float*>(raySphereStructureOfArrays.directionY.getAlignedPtr())[index] = static_cast<float>(normalizedVelocity(1));
			reinterpret_cast<float*>(raySphereStructureOfArrays.directionZ.getAlignedPtr())[index] = static_cast<float>(normalizedVelocity(2));
		}

		// fill radius
		for( int index = 0; index < fastParticles.getCount(); index++ ) {
			reinterpret_cast<float*>(raySphereStructureOfArrays.r.getAlignedPtr())[index] = collisionSphereRadius;
		}
	}

	void doBatchedCollisionTests(RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays) {
		for( int batchCounter = 0; batchCounter < raySphereStructureOfArrays.getNumberOfElements()/batchSize; batchCounter++) {
			raySphereForRelativeSphereVectorized(
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionX.getAlignedPtr())[batchCounter*batchSize]),
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionY.getAlignedPtr())[batchCounter*batchSize]),
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionZ.getAlignedPtr())[batchCounter*batchSize]),

				&(reinterpret_cast<float*>(raySphereStructureOfArrays.directionX.getAlignedPtr())[batchCounter*batchSize]),
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.directionY.getAlignedPtr())[batchCounter*batchSize]),
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.directionZ.getAlignedPtr())[batchCounter*batchSize]),
				
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.r.getAlignedPtr())[batchCounter*batchSize]),

				&(reinterpret_cast<float*>(raySphereStructureOfArrays.intersect.getAlignedPtr())[batchCounter*batchSize]),

				&(reinterpret_cast<float*>(raySphereStructureOfArrays.plus.getAlignedPtr())[batchCounter*batchSize]),
				&(reinterpret_cast<float*>(raySphereStructureOfArrays.minus.getAlignedPtr())[batchCounter*batchSize])
			);
		}
	}

	// - check for positive intersect, if true, check if plus or minus is in the length of the ray
	void checkForCollisions(const RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays, const Array<SharedPointer<FastParticle>> &fastParticles, Array<bool> &collisionTestResult) {
		for( int index = 0; index < fastParticles.getCount(); index++ ) {
			SharedPointer<FastParticle> currentParticle = fastParticles[index];

			float intersect = reinterpret_cast<float*>(raySphereStructureOfArrays.intersect.getAlignedPtr())[index];
			float lengthOfRay = currentParticle->velocity.norm(); // length

			// we choose plus here
			// TODO< fix me, check if this is the right direction >
			float intersectionLength = reinterpret_cast<float*>(raySphereStructureOfArrays.plus.getAlignedPtr())[index];

			collisionTestResult[index] = intersect >= 0.0f && intersectionLength > 0.0f && intersectionLength < lengthOfRay;
		}

	}
}