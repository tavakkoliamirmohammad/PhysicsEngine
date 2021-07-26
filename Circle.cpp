#include "Circle.h"

Circle::Circle(float radius, float mass, glm::vec2 origin) : radius(radius), origin(origin),
                                                             Shape(mass, ShapeType::External) {
    this->CalculateInertia();
}

const glm::vec2 &Circle::GetOrigin() const {
    return origin;
}

void Circle::SetOrigin(const glm::vec2 &origin) {
    this->origin = origin;
}

void Circle::CalculateInertia() {
    float r_squared = this->radius * this->radius;
    this->momentOfInertia = glm::pi<float>() / 2.0 * r_squared * r_squared;
}


void Circle::SetRadius(float radius) {
    this->radius = radius;
}

glm::vec2 Circle::GetCentroid() {
    glm::vec2 centroid(0);
    auto center = position;
    centroid += center;
    return centroid;
}
