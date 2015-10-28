#pragma once

#include "bindings/SharedPointer.h"
#include "physics/PhysicsBody.h"
#include "physics/FastParticle.hpp"

class CollisionCallback {
public:
    struct Source {
        enum class EnumType {
            FASTPARTICLE
        };

        EnumType type;

        SharedPointer<FastParticle> fastParticle;
    };

    // TODO< position, normal >
    virtual void call(SharedPointer<PhysicsBody> physicsBody, const Source &source) = 0;
};
