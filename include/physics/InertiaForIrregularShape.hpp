#pragma once

#include <Eigen/Dense>







template<typename ScalarType>
ScalarType powConstIntExponent(const ScalarType base, const unsigned int exponent) {
    if( exponent == 0 ) {
        return 1.0;
    }
    
    ScalarType result = base;
    
    for(unsigned int i = 1; i < exponent; i++ ) {
        result *= base;
    }
    
    return result;
}









// TODO< move to file VectorUtilities.hpp >

template<typename ScalarType, typename VectorType>
ScalarType calculateMaskedQuadraticDotProductOfVector(const VectorType &vector, const bool maskX, const bool maskY, const bool maskZ) {
    return calculateMaskedExponentProductOfVector(vector, maskX, maskY, maskZ, 2);
}

template<typename ScalarType, typename VectorType>
ScalarType calculateMaskedDotProductOfVector(const VectorType &vector, const bool maskX, const bool maskY, const bool maskZ) {
    return calculateMaskedExponentProductOfVector(vector, maskX, maskY, maskZ, 1);
}

template<typename ScalarType, typename VectorType>
ScalarType calculateMaskedExponentProductOfVector(const VectorType &vector, const bool maskX, const bool maskY, const bool maskZ, unsigned int exponent) {
    ScalarType result;
		
    bool remainingMask[3];
    remainingMask[0] =maskX;
    remainingMask[1] =maskY;
    remainingMask[2] =maskZ;
        
    // scan for first masked value
    for( unsigned int i = 0; i < 3; i++ ) {
        if (remainingMask[i]) {
            result = powConstIntExponent(vector(i), exponent);
            remainingMask[i] = false;
            break;
        }
    }
      
    for( unsigned int i = 0; i < 3; i++ ) {
        if (remainingMask[i]) {
            result += powConstIntExponent(vector(i), exponent);
        }
    }

    return result;
}







// http://www.brighthubengineering.com/machine-design/48333-calculate-mass-moment-of-inertia-for-irregular-geometry-in-six-steps/
// (in the integral the r2 is r^2)

// tensor calculation
// http://farside.ph.utexas.edu/teaching/336k/Newtonhtml/node64.html

template<typename ScalarType>
struct InertiaForIrregularShape {
	typedef Matrix<ScalarType, 3, 1> VectorType;

	// axis needs to be normalized!
	Matrix<NumericType, 3, 3> integrateAsTensor(IVolume<VectorType> *volume, VectorType &min, VectorType &max, VectorType &segmentSize, NumericType &density) {
		const ScalarType massOfSegment = (segmentSize*density).getLength();

		ScalarType ixx = 0.0, iyy = 0.0, izz = 0.0;
		ScalarType ixyNeg = 0.0, iyzNeg = 0.0, ixzNeg = 0.0;

		for (ScalarType iz = min.z; iz < max.z; iz += segmentSize.z) {
			for (ScalarType iy = min.y; iy < max.y; iy += segmentSize.y) {
				for (ScalarType ix = min.x; ix < max.x; ix += segmentSize.x) {
					const VectorType segmentPosition << ix, iy, iz;

					if (!volume->getAt(segmentPosition)) {
						continue;
					}
					// else we are here

					ixx += (calculateMaskedQuadraticDotProductOfVector(segmentPosition, false, true, true)*massOfSegment);
					iyy += (calculateMaskedQuadraticDotProductOfVector(segmentPosition, true, false, true)*massOfSegment);
					izz += (calculateMaskedQuadraticDotProductOfVector(segmentPosition, true, true, false)*massOfSegment);
					ixyNeg += (calculateMaskedDotProductOfVector(segmentPosition, true, true, false)*massOfSegment);
					iyzNeg += (calculateMaskedDotProductOfVector(segmentPosition, false, true, true)*massOfSegment);
					ixzNeg += (calculateMaskedDotProductOfVector(segmentPosition, true, false, true)*massOfSegment);
				}
			}
		}

		const ScalarType ixy = -ixyNeg, iyz = -iyzNeg, ixz = -ixzNeg;
		const ScalarType iyx = ixy, izy = iyz, izx = ixz;

		Matrix<NumericType, 3, 3> resultMatrix;
		resultMatrix <<
			ixx, ixy, ixz,
			iyx, iyy, iyz,
			izx, izy, izz;

		return resultMatrix;
	}

	protected: static ScalarType calculateDistanceFromAxis(const VectorType &axis, const VectorType &position) {
		const ScalarType dotResult = VectorType::dot(axis, position);
		const VectorType positionPerpendicular = axis.getScaled(dotResult);
		const ScalarType distance = (position - positionPerpendicular).getLength();
		return distance;
	}
};
