#pragma once

#include "Object.h"
#include <tuple>

enum class CollisionType {
    BoxCircle, CircleCircle, BoxBox
};

struct CollisionInfo {
    Object *object1;
    Object *object2;
    glm::vec2 penetrationPoint;
    glm::vec2 contactPoint;
    glm::vec2 normal;
    CollisionType collisionType;
    float penetrationDepth;
    bool isCollided;
};

class CollisionDetector {
public:
    static bool ShapeContainsPoint(Object *object, glm::vec2 p);

    CollisionInfo Detect(Object *object1, Object *object2);

    std::vector<CollisionInfo> Detect(std::vector<Object *> *objects);
};