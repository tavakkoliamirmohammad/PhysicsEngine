#include "Box2D.h"

Box2D::Box2D(float h, float w, float mass, ShapeType type) :
        height(h), width(w), Shape(mass, type) {
    vertices.emplace_back(-w / 2, -h / 2);
    vertices.emplace_back(w / 2, -h / 2);
    vertices.emplace_back(w / 2, h / 2);
    vertices.emplace_back(-w / 2, h / 2);
    CalculateBoxInertia();
}

std::vector<glm::vec2> Box2D::GetVertices() {
    std::vector<glm::vec2> result;
    auto rm = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
    for (auto v : this->vertices) {
        auto nv = rm * glm::vec4(v, 0, 1);
        result.emplace_back(nv.x + position.x, nv.y + position.y);
    }
    return result;
}

glm::vec2 Box2D::GetCentroid() {
    glm::vec2 centroid(0);
    auto vertexes = GetVertices();
    for (auto v : vertexes) centroid += v;
    return centroid / (float) vertexes.size();
}

void Box2D::CalculateBoxInertia() {
    float m = mass;
    float w = width;
    float h = height;
    momentOfInertia = m * (w * w + h * h) / 12;
}
