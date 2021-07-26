#include "Circle.h"

Circle::Circle(float radius, float mass) : radius(radius), Object(mass, ObjectType::External) {
    this->CalculateInertia();
}

void Circle::SetRadius(float radius) {
    this->radius = radius;
}

glm::vec2 Circle::GetCenter() {
    return position;
}

void Circle::CalculateInertia() {
    float r_squared = this->radius * this->radius;
    this->momentOfInertia = glm::pi<float>() / 2.0 * r_squared * r_squared;
}

glm::vec2 Circle::GetCentroid() {
    glm::vec2 centroid(0);
    auto center = position;
    centroid += center;
    return centroid;
}

bool Circle::ContainsPoint(glm::vec2 v) {
    return glm::length(GetCenter() - v) <= GetRadius();
}

CollisionInfo Circle::DetectCollision(Circle *circle1, Circle *circle2) {
    glm::vec2 separationVector = circle1->GetCenter() - circle2->GetCenter();
    float penetrationDepth = circle1->GetRadius() + circle2->GetRadius() - glm::length(separationVector);
    return CollisionInfo(
            {circle1, circle2, glm::vec2(0),
             glm::vec2(0),
             separationVector / glm::length(separationVector),
             CollisionType::CircleCircle,
             penetrationDepth > 0 ? penetrationDepth : 0,
             glm::length(separationVector) <= circle1->GetRadius() + circle2->GetRadius()});;
}