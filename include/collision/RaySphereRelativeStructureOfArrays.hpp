#pragma once

#include "memory/AlignedAllocator.hpp"

struct RaySphereRelativeStructureOfArrays {
	AlignedAllocator<32, float> cachedRelativePositionX;
	AlignedAllocator<32, float> cachedRelativePositionY;
	AlignedAllocator<32, float> cachedRelativePositionZ;
	AlignedAllocator<32, float> directionX;
	AlignedAllocator<32, float> directionY;
	AlignedAllocator<32, float> directionZ;
	AlignedAllocator<32, float> r;

	AlignedAllocator<32, float> intersect;

	AlignedAllocator<32, float> plus, minus;

    RaySphereRelativeStructureOfArrays();

	// batch ize is the size of the batch which gets worked on in the collision function (ray vs sphere), should be a exponent of 2
	void resizeToSizeIfRequiredWithBatchSize(unsigned size, unsigned batchSize);

    unsigned getNumberOfElements() const;
protected:
    unsigned realSize;
};
