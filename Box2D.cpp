#include "Box2D.h"


Box2D::Box2D(float h, float w, float mass, BoxType type):
    shapeType(type)
{
    CalculateBoxInertia();
}

std::vector<glm::vec2> Box2D::GetVertices()
{
    std::vector<glm::vec2> result;

    auto rm = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
    for (auto v : this->vertices)
    {
        auto nv = rm * glm::vec4(v, 0, 1);
        result.emplace_back(nv.x + position.x, nv.y + position.y);
    }

    return result;
}

glm::vec2 Box2D::GetCentroid()
{
    glm::vec2 centroid(0);
    auto vertices = GetVertices();
    for (auto v : vertices) centroid += v;
    return centroid / (float)vertices.size();
}

void Box2D::CalculateBoxInertia()
{
    float m = shape.mass;
    float w = shape.width;
    float h = shape.height;
    shape.momentOfInertia = m * (w * w + h * h) / 12;
}
