#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.h"

class Box2D : public Object {
public:

    Box2D(float h, float w, float mass, ObjectType type = ObjectType::External);

    std::vector<glm::vec2> GetVertices();

    glm::vec2 GetCentroid() override;

    void CalculateInertia() override;

private:
    float width;
    float height;
    std::vector<glm::vec2> vertices;
};
