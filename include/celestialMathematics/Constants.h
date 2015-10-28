// Written by SquareOfTwo, Github PtrMan, email:robertw89@googlemail.com

#pragma once

// TODO< if windows vs2015 is supported rewrite to constexpr >
namespace Constants {
  static const double GravitationalConstant = 0.0000000000667384;
  
  // not Celestial
  static const double StefanBoltzmannConstant = 0.00000005670;
  
  static const double LightSecond = 299792458.0;
  static const double AstronomicalUnit = LightSecond * 499.004784;
  static const double LightYear = AstronomicalUnit * 63239.7263;
};
