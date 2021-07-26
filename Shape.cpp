#include "Shape.h"

using namespace std;

Shape::Shape(float mass, ShapeType type) :
        shapeType(type),
        position(0), linearVelocity(0), momentOfInertia(0), mass(mass), force(0),
        angle(0), angularVelocity(0), torque(0) {

}

void Shape::MoveTo(float x, float y, bool relative) {
    if (relative) {
        position.x += x;
        position.y += y;
    } else {
        position.x = x;
        position.y = y;
    }
}

void Shape::SetLinearVelocity(float vx, float vy) {
    linearVelocity.x = vx;
    linearVelocity.y = vy;
}

void Shape::SetForce(float fx, float fy) {
    force.x = fx;
    force.y = fy;
}

void Shape::Rotate(float angle, bool relative) {
    if (relative) this->angle += angle;
    else this->angle = angle;
}

void Shape::SetAngularVelocity(float av) {
    angularVelocity = av;
}

void Shape::SetTorque(float torque) {
    this->torque = torque;
}

void Shape::ChangeMass(float mass, bool relative) {
    if (relative) this->mass += mass;
    else this->mass = mass;

    if (this->mass < 1) this->mass = 1;
}

float Shape::GetAngularMass(glm::vec2 r, glm::vec2 n) const {
    auto c = glm::cross(glm::vec3(r, 0), glm::vec3(n, 0)).z;
    auto m = momentOfInertia / c * c;
    return m;
}

