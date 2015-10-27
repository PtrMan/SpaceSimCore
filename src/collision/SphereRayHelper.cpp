#include "collision/SphereRayHelper.hpp"

#include "physics/FastParticle.hpp"
#include "collision/RaySphereRelativeStructureOfArrays.hpp"
#include "math/RaySphere.hpp"
#include "bindings/Array.h"
#include "bindings/SharedPointer.h"

void SphereRayHelper::transferFastParticlesToRaySphereSoa(const Array<SharedPointer<FastParticle>> &fastParticles, const float collisionSphereRadius, const Eigen::Matrix<double, 3, 1> &collisionSpherePosition, RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays) {
    float *cachedRelativePositionXAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionX.getAlignedPtr());
    float *cachedRelativePositionYAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionY.getAlignedPtr());
    float *cachedRelativePositionZAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.cachedRelativePositionZ.getAlignedPtr());

    float *directionXAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.directionX.getAlignedPtr());
    float *directionYAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.directionY.getAlignedPtr());
    float *directionZAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.directionZ.getAlignedPtr());

    float *rAligned = reinterpret_cast<float*>(raySphereStructureOfArrays.r.getAlignedPtr());

	raySphereStructureOfArrays.resizeToSizeIfRequiredWithBatchSize(fastParticles.getCount(), batchSize);

	// calculate difference between ray positions and sphere position
	for( int index = 0; index < fastParticles.getCount(); index++ ) {
		SharedPointer<FastParticle> currentParticle = fastParticles[index];

		const Eigen::Matrix<double, 3, 1> positionDifference = currentParticle->currentPosition - collisionSpherePosition;
        cachedRelativePositionXAligned[index] = static_cast<float>(positionDifference(0));
        cachedRelativePositionYAligned[index] = static_cast<float>(positionDifference(1));
        cachedRelativePositionZAligned[index] = static_cast<float>(positionDifference(2));
	}

	// calculate (normalized) directions of spheres
	for( int index = 0; index < fastParticles.getCount(); index++ ) {
		SharedPointer<FastParticle> currentParticle = fastParticles[index];

		const Eigen::Matrix<double, 3, 1> normalizedVelocity = currentParticle->velocity.normalized();
        directionXAligned[index] = static_cast<float>(normalizedVelocity(0));
        directionYAligned[index] = static_cast<float>(normalizedVelocity(1));
        directionZAligned[index] = static_cast<float>(normalizedVelocity(2));
	}

	// fill radius
	for( int index = 0; index < fastParticles.getCount(); index++ ) {
        rAligned[index] = collisionSphereRadius;
	}
}

void SphereRayHelper::doBatchedCollisionTests(RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays) {
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


void SphereRayHelper::checkForCollisions(const RaySphereRelativeStructureOfArrays &raySphereStructureOfArrays, const Array<SharedPointer<FastParticle>> &fastParticles, Array<int> &collisionTestResult) {
    int numberOfFastParticles = fastParticles.getCount();

    for( int index = 0; index < numberOfFastParticles; index++ ) {
		SharedPointer<FastParticle> currentParticle = fastParticles[index];

		float intersect = reinterpret_cast<float*>(raySphereStructureOfArrays.intersect.getAlignedPtr())[index];
		float lengthOfRay = currentParticle->velocity.norm();

		// we choose plus here
		// TODO< fix me, check if this is the right direction >
		float intersectionLength = reinterpret_cast<float*>(raySphereStructureOfArrays.plus.getAlignedPtr())[index];

		collisionTestResult[index] = intersect >= 0.0f && intersectionLength > 0.0f && intersectionLength < lengthOfRay;
	}

}
