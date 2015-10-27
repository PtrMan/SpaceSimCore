#pragma once

#include "eigen/Dense"

namespace InertiaHelper {
	Eigen::Matrix<double, 3, 3> calcInertiaTensorForSolidEllipsoid(double mass, double a, double b, double c);
	Eigen::Matrix<double, 3, 3> calcInertiaTensorForSolidSphere(double mass, double radius);
}
