// Written by SquareOfTwo, Github PtrMan, email:robertw89@googlemail.com

#pragma once

// see https://en.wikipedia.org/wiki/Hohmann_transfer_orbit
namespace HohmannTransfer {
    double hohmannTransferDeltaV1(const double centerMass, const double r1, const double r2);
    double hohmannTransferDeltaV2(const double centerMass, const double r1, const double r2);
}
