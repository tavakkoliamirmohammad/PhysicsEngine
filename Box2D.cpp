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

    for (int i = 0; i < n1; i++) {
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

CollisionInfo Box2D::DetectCollision(Box2D *box1, Box2D *box2) {
    CollisionInfo info({box1, box2, glm::vec2(0), glm::vec2(0), glm::vec2(0), CollisionType::BoxBox, 0, false});

    auto box1_vertices = box1->GetVertices();
    auto box2_vertices = box2->GetVertices();

    auto n1 = box1_vertices.size();
    auto n2 = box2_vertices.size();

    float minPenetration = 1e10;
    bool isInternal = box1->GetShapeType() == ObjectType::Internal;

    bool isShapeInFrontOfEdge = true;

    for (int i = 0; i < n1; i++) {
        auto a = box1_vertices[i];
        auto b = box1_vertices[(i + 1) % n1];

        auto n = GetEdgeNormal(a - b);
        if (isInternal) n = -n;
        else isShapeInFrontOfEdge = true;

        float maxPenetration = 0;
        CollisionInfo max_info({box1, box2, glm::vec2(0), glm::vec2(0), glm::vec2(0), CollisionType::BoxBox, 0, false});

        for (int j = 0; j < n2; j++) {
            auto v = box2_vertices[j];

            if (glm::dot(v - a, n) < 0) {
                isShapeInFrontOfEdge = false;

                auto p = ProjectToEdge(a, b, v);
                //if (IsPointBetweenTwoPoint(a, b, p))
                {
                    auto penetration = glm::length(p - v);

                    if (penetration > maxPenetration) {
                        maxPenetration = penetration;
                        max_info = CollisionInfo({box1, box2, v, p, n, CollisionType::BoxBox, penetration, true});
                    }
                }
            }
        }

        if (isShapeInFrontOfEdge && !isInternal) {
            info.isCollided = false;
            return info;
        } else if (max_info.isCollided) {
            if (maxPenetration < minPenetration) {
                minPenetration = maxPenetration;
                info = max_info;
            }
        }
    }

    if (isShapeInFrontOfEdge) info.isCollided = false;
    return info;
}

glm::vec2 Box2D::ProjectToEdge(glm::vec2 v1, glm::vec2 v2, glm::vec2 p) {
    auto e1 = v2 - v1;
    auto e2 = p - v1;
    float valDp = glm::dot(e1, e2);
    float lenE1 = glm::length(e1);
    float lenE2 = glm::length(e2);
    float cos = valDp / (lenE1 * lenE2);
    float projLenOfLine = cos * lenE2;
    glm::vec2 projPoint = v1 + e1 / lenE1 * projLenOfLine;
    return projPoint;
}
