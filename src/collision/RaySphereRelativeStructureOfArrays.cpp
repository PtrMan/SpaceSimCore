#include "collision/RaySphereRelativeStructureOfArrays.hpp"

RaySphereRelativeStructureOfArrays::RaySphereRelativeStructureOfArrays() {
    realSize = 0;
}

void RaySphereRelativeStructureOfArrays::resizeToSizeIfRequiredWithBatchSize(unsigned size, unsigned batchSize) {
    realSize = ((size / batchSize) + 1) * batchSize;

	cachedRelativePositionX.resizeIfRequiredTo(realSize);
	cachedRelativePositionY.resizeIfRequiredTo(realSize);
	cachedRelativePositionZ.resizeIfRequiredTo(realSize);

	directionX.resizeIfRequiredTo(realSize);
	directionY.resizeIfRequiredTo(realSize);
	directionZ.resizeIfRequiredTo(realSize);

	intersect.resizeIfRequiredTo(realSize);

	plus.resizeIfRequiredTo(realSize);
	minus.resizeIfRequiredTo(realSize);


	r.resizeIfRequiredTo(realSize);
}

unsigned RaySphereRelativeStructureOfArrays::getNumberOfElements() const {
    return realSize;
}
