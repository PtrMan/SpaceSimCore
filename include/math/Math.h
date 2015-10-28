// Written by SquareOfTwo, Github PtrMan, email:robertw89@googlemail.com

#pragma once

namespace Math {
	// TODO< if windows vs2015 is supported rewrite to constexpr >
  // http://stackoverflow.com/questions/16443682/c-power-of-integer-template-meta-programming
  template<class T>
  inline T exponentInteger(const T base, unsigned const exponent) {
    // (parentheses not required in next line)
    return (exponent == 0) ? 1 : (base * exponentInteger(base, exponent-1));
  }
};
