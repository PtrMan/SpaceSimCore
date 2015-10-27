#pragma once

#include "eigen/Dense"

// see https://en.wikipedia.org/wiki/List_of_moments_of_inertia

namespace InertiaHelper {

Eigen::Matrix<double, 3, 3> calcInertiaTensorForSolidEllipsoid(double mass, double a, double b, double c) {
    Eigen::Matrix<double, 3, 3> result;

    result <<
		(1.0/5.0)*(b*b + c*c), 0, 0,
		0, (1.0/5.0)*(a*a + c*c), 0,
		0, 0, (1.0/5.0)*(a*a + b*b);
	;

	return result;
}

Eigen::Matrix<double, 3, 3> calcInertiaTensorForSolidSphere(double mass, double radius) {
	return calcInertiaTensorForSolidEllipsoid(mass, radius, radius, radius);
}

}
