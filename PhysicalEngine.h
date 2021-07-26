#pragma once

#include "CollisionDetector.h"
#include "CollisionResolver.h"

class PhysicalEngine {
public:

    PhysicalEngine(bool resolveCollision) :
            resolveCollision(resolveCollision),
            RigidBodies(nullptr),
            isChangedRigidBodies(false) {}

    void AddRigidBody(Object *object);

    void SetRigidBodies(std::vector<Object *> *);

    std::vector<CollisionInfo> CurrentCollisionInfo();

    void SetResolverAcivity(bool resolveCollision);

    inline void SetChangedRigidBodies(bool changed) { isChangedRigidBodies = changed; }

    void Update(float dt);

private:
    CollisionDetector detector;
    CollisionResolver resolver;
    std::vector<Object *> *RigidBodies;
    std::vector<CollisionInfo> currentCollisionInfo;
    bool resolveCollision;
    bool isChangedRigidBodies;
};