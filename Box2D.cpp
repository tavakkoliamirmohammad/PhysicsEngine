#include "Box2D.h"

Box2D::Box2D(float h, float w, float mass, ObjectType type) :
        height(h), width(w), Object(mass, type) {
    vertices.emplace_back(-w / 2, -h / 2);
    vertices.emplace_back(w / 2, -h / 2);
    vertices.emplace_back(w / 2, h / 2);
    vertices.emplace_back(-w / 2, h / 2);
    this->CalculateInertia();
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

void Box2D::CalculateInertia() {
    float m = mass;
    float w = width;
    float h = height;
    momentOfInertia = m * (w * w + h * h) / 12;
}

bool Box2D::ContainsPoint(glm::vec2 v) {
    auto box1_vertices = GetVertices();
    auto n1 = box1_vertices.size();

    for (int i = 0; i < n1; i++)
    {
        auto a = box1_vertices[i];
        auto b = box1_vertices[(i + 1) % n1];

        auto n = GetEdgeNormal(a - b);

        bool isShapeInFrontOfEdge = true;

        if (glm::dot(v - a, n) > 0)
            return false;
    }

    return true;
}

glm::vec2 Box2D::GetEdgeNormal(glm::vec2 edge) {
    edge /= glm::length(edge);
    auto n = glm::cross(glm::vec3(0, 0, 1), glm::vec3(edge, 0));
    return glm::vec2(n.x, n.y);
}
