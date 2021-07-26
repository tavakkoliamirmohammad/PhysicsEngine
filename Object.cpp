#include "Object.h"

using namespace std;

Object::Object(float mass, ObjectType type) :
        shapeType(type),
        position(0), linearVelocity(0), momentOfInertia(0), mass(mass), force(0),
        angle(0), angularVelocity(0), torque(0) {
}

void Object::MoveTo(float x, float y, bool relative) {
    if (relative) {
        position.x += x;
        position.y += y;
    } else {
        position.x = x;
        position.y = y;
    }
}

void Object::SetLinearVelocity(float vx, float vy) {
    linearVelocity.x = vx;
    linearVelocity.y = vy;
}

void Object::SetForce(float fx, float fy) {
    force.x = fx;
    force.y = fy;
}

void Object::Rotate(float angle, bool relative) {
    if (relative) this->angle += angle;
    else this->angle = angle;
}

void Object::SetAngularVelocity(float av) {
    angularVelocity = av;
}

void Object::SetTorque(float torque) {
    this->torque = torque;
}

void Object::ChangeMass(float mass, bool relative) {
    if (relative) this->mass += mass;
    else this->mass = mass;

    if (this->mass < 1) this->mass = 1;
}

float Object::GetAngularMass(glm::vec2 r, glm::vec2 n) const {
    auto c = glm::cross(glm::vec3(r, 0), glm::vec3(n, 0)).z;
    auto m = momentOfInertia / c * c;
    return m;
}

void Object::CalculateInertia() {

}

