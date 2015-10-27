// difference between ray start and sphere
// float3 pdiff = p - spherePosition;

// g++ -I /home/r0b3/libs/eigen -c -ffast-math -march=corei7 -O3 -std=c++0x -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=9 ./RaySphere2.cpp

#pragma once

/*
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


float dotProduct(float ax, float ay, float az, float bx, float by, float bz);

void raySphereForRelativeSphereVectorized(float *__restrict__ relativePositionX, float *__restrict__ relativePositionY, float *__restrict__ relativePositionZ, float *__restrict__ directionX, float *__restrict__ directionY, float *__restrict__ directionZ, float *__restrict__ r, float *__restrict__ intersect, float *__restrict__ plus, float *__restrict__ minus);


/*
void raySphereForRelativeSpherePacked(const Packed<4, Eigen::Matrix<float, 3, 1>> &relativePosition, const Packed<4, Eigen::Matrix<float, 3, 1>> &direction, const Packed<4, float> &r, Packed<4, float> &intersect, Packed<4, float> &plus, Packed<4, float> &minus) {
	for( unsigned i = 0; i < 4; i++ ) {
		raySphereForRelativeSphere(relativePosition.data[i], direction.data[i], r.data[i], intersect.data[i], plus.data[i], minus.data[i]);
	}
}
*/