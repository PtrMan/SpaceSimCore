// difference between ray start and sphere
// float3 pdiff = p - spherePosition;

// g++ -I /home/r0b3/libs/eigen -c -ffast-math -march=corei7 -O3 -std=c++0x -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=9 ./RaySphere2.cpp

#include <cmath>

/*
 *
template<unsigned Size, typename Type>
struct Packed {
	Type data[Size];

	+*
	Type *getAligned16Ptr() {
		return static_cast<float*>(__builtin_assume_aligned(&(data[0]), 16));
	}*+
};
*/

/*
void raySphereForRelativeSphere(const Eigen::Matrix<float, 3, 1> &relativePosition, const Eigen::Matrix<float, 3, 1> &direction, const float &r, float &intersect, float &plus, float &minus) {
	float a = 1.0f; // dir*dir, is 1.0 because dir is assumed to be normalized
	float b = 2.0f * relativePosition.dot(direction);
	float c = relativePosition.dot(relativePosition) - r * r;

	float divTwoA = (1.0f/a)*0.5f;

	float inSqrt = intersect = b*b - 4.0f*a*c;
	float sqrtResult = sqrt(inSqrt);

	plus = (-b + sqrtResult) * divTwoA;
	minus = (-b - sqrtResult) * divTwoA;
}
*/


float dotProduct(float ax, float ay, float az, float bx, float by, float bz) {
	return ax*bx + ay*by + az*bz;
}

void raySphereForRelativeSphereVectorized(float *__restrict__ relativePositionX, float *__restrict__ relativePositionY, float *__restrict__ relativePositionZ, float *__restrict__ directionX, float *__restrict__ directionY, float *__restrict__ directionZ, float *__restrict__ r, float *__restrict__ intersect, float *__restrict__ plus, float *__restrict__ minus) {
    const unsigned BATCHSIZE = 16;

    float *alignedRelativePositionX = static_cast<float*>(__builtin_assume_aligned(relativePositionX, 32));
	float *alignedRelativePositionY = static_cast<float*>(__builtin_assume_aligned(relativePositionY, 32));
	float *alignedRelativePositionZ = static_cast<float*>(__builtin_assume_aligned(relativePositionZ, 32));

	float *alignedDirectionX = static_cast<float*>(__builtin_assume_aligned(directionX, 32));
	float *alignedDirectionY = static_cast<float*>(__builtin_assume_aligned(directionY, 32));
	float *alignedDirectionZ = static_cast<float*>(__builtin_assume_aligned(directionZ, 32));

	float *alignedIntersect = static_cast<float*>(__builtin_assume_aligned(intersect, 32));


	float *alignedPlus = static_cast<float*>(__builtin_assume_aligned(plus, 32));
	float *alignedMinus = static_cast<float*>(__builtin_assume_aligned(minus, 32));

	float *alignedR = static_cast<float*>(__builtin_assume_aligned(r, 32));

    float a[BATCHSIZE] __attribute__ ((aligned (32)));
    float b[BATCHSIZE] __attribute__ ((aligned (32)));
    float c[BATCHSIZE] __attribute__ ((aligned (32)));
    float divTwoA[BATCHSIZE] __attribute__ ((aligned (32)));
    float inSqrt[BATCHSIZE] __attribute__ ((aligned (32)));
    float sqrtResult[BATCHSIZE] __attribute__ ((aligned (32)));

	
    for( int vectorIndex = 0; vectorIndex < BATCHSIZE; vectorIndex++ ) {
		a[vectorIndex] = 1.0f;
		b[vectorIndex] = 2.0f * dotProduct(alignedRelativePositionX[vectorIndex], alignedRelativePositionY[vectorIndex], alignedRelativePositionZ[vectorIndex],  alignedDirectionX[vectorIndex], alignedDirectionY[vectorIndex], alignedDirectionZ[vectorIndex]);
		c[vectorIndex] = dotProduct(alignedRelativePositionX[vectorIndex], alignedRelativePositionY[vectorIndex], alignedRelativePositionZ[vectorIndex],  alignedRelativePositionX[vectorIndex], alignedRelativePositionY[vectorIndex], alignedRelativePositionZ[vectorIndex]) - alignedR[vectorIndex]*alignedR[vectorIndex];

		divTwoA[vectorIndex] = (1.0f / a[vectorIndex])*0.5f;

		
		inSqrt[vectorIndex] = alignedIntersect[vectorIndex] = b[vectorIndex]*b[vectorIndex] - 4.0f*a[vectorIndex]*c[vectorIndex];
		
		sqrtResult[vectorIndex] = sqrt(inSqrt[vectorIndex]);

		alignedPlus[vectorIndex] = (-b[vectorIndex] + sqrtResult[vectorIndex]) * divTwoA[vectorIndex];
		alignedMinus[vectorIndex] = (-b[vectorIndex] - sqrtResult[vectorIndex]) * divTwoA[vectorIndex];
	}
}


/*
void raySphereForRelativeSpherePacked(const Packed<4, Eigen::Matrix<float, 3, 1>> &relativePosition, const Packed<4, Eigen::Matrix<float, 3, 1>> &direction, const Packed<4, float> &r, Packed<4, float> &intersect, Packed<4, float> &plus, Packed<4, float> &minus) {
	for( unsigned i = 0; i < 4; i++ ) {
		raySphereForRelativeSphere(relativePosition.data[i], direction.data[i], r.data[i], intersect.data[i], plus.data[i], minus.data[i]);
	}
}
*/
