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