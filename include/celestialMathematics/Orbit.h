// Written by SquareOfTwo, Github PtrMan, email:robertw89@googlemail.com

#pragma once

struct Orbit {
  static double calcVelocityOfCircularOrbit(double centerMass, double radius);
  static double calcVelocityOfCenteredEllipticalOrbit(double centerMass, double lengthOfSemimajorAxis, double radius);
  
  static double calculateOrbitalPeriod(const double centerMass, const double lengthOfSemimajorAxis);
  
  static double calculateForceBetweenObjectsByRadius(const double massA, const double radiusA, const double massB, const double radiusB);

  static double calculateForceBetweenObjectsByDistance(const double massA, const double massB, const double distance);


  // w : initial spin rate in radiants per second
  // a : semimajor axis of motion of satellite around planet/bigger body
  // Q : dissipation function of the satellite
  // G : gravitational costant
  // ms: mass of satellite
  // mc: mass of center
  // R : mean radius of satellite

  // p : density of satellite
  // mu: riggidity of satellite
  static double calcTidalLockingTimeForSphericalSatellite(const double w, const double a, const double Q, const double ms, const double mc, const double R, const double p, const double mu);

  // https://en.wikipedia.org/wiki/Tidal_locking#Timescale
  // w : initial spin rate in radiants per second
  // a : semimajor axis of motion of satellite around planet/bigger body
  // I : moment of inertia of the satellite (typical for sphere, 0.4ms*R^2)
  // Q : dissipation function of the satellite
  // G : gravitational costant
  // ms: mass of satellite
  // mc: mass of center
  // R : mean radius of satellite

  // p : density of satellite
  // mu: riggidity of satellite
  static double calcTidalLockingTime(const double w, const double a, const double I, const double Q, const double ms, const double mc, const double R, const double p, const double mu);

  static double calcK2(const double p, const double g, const double mu, const double R);

  static double calcSurfaceGravity(const double mass, const double r);

};
