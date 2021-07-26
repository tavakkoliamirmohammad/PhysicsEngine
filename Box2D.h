#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.h"
#include "CollisionDetector.h"
#include "Circle.h"

class Box2D : public Object {
public:

    Box2D(float h, float w, float mass, ObjectType type = ObjectType::External);

    std::vector<glm::vec2> GetVertices();

    glm::vec2 GetCentroid() override;

    void CalculateInertia() override;

    bool ContainsPoint(glm::vec2 v) override;

    static glm::vec2 GetEdgeNormal(glm::vec2 edge);

    static CollisionInfo DetectCollision(Box2D *box1, Box2D *box2);

    static glm::vec2 ProjectToEdge(glm::vec2 v1, glm::vec2 v2, glm::vec2 p);


private:
    float width;
    float height;
    std::vector<glm::vec2> vertices;
};
