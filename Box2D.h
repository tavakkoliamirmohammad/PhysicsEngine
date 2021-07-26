#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shape.h"

enum class BoxType {
    Internal,
    External
};

class Box2D : public Shape {
public:

    Box2D(float h, float w, float mass, BoxType type = BoxType::External);

    inline BoxType GetShapeType() { return this->shapeType; }

    std::vector<glm::vec2> GetVertices();

    glm::vec2 GetCentroid() override;


private:

    void CalculateBoxInertia();

    float width;
    float height;
    std::vector<glm::vec2> vertices;
    BoxType shapeType;
};
