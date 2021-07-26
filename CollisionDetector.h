#pragma once

#include "Object.h"
#include <tuple>

struct CollisionInfo {
    Object *object1;
    Object *object2;
    glm::vec2 penetrationPoint;
    glm::vec2 contactPoint;
    glm::vec2 normal;
    float penetrationDepth;
    bool isCollided;
};

class CollisionDetector {
public:
    static bool ShapeContainsPoint(Object *object, glm::vec2 p);

    static glm::vec2 ProjectToEdge(glm::vec2 v1, glm::vec2 v2, glm::vec2 p);


    CollisionInfo Detect(Object *object1, Object *object2);

    std::vector<CollisionInfo> Detect(std::vector<Object *> *objects);
};