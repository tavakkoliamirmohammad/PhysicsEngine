#ifndef PHYSICALENGINE_OBJECT_H
#define PHYSICALENGINE_OBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

enum class ObjectType {
    Internal,
    External
};

class Object {
public:

    Object(float mass, ObjectType type);

    void MoveTo(float x, float y, bool relative = false);

    void SetLinearVelocity(float vx, float vy);

    void SetForce(float fx, float fy);

    void Rotate(float angle, bool relative = false);

    void SetAngularVelocity(float av);

    void SetTorque(float torque);

    inline glm::vec2 GetPosition() { return this->position; }

    inline glm::vec2 GetLinearVelocity() { return this->linearVelocity; }

    inline glm::vec2 GetForce() { return this->force; }

    inline float GetAngle() const { return this->angle; }

    inline float GetAngularVelocity() const { return this->angularVelocity; }

    virtual glm::vec2 GetCentroid() = 0;

    virtual void CalculateInertia();

    inline ObjectType GetShapeType() { return this->shapeType; }

    inline float GetTorque() const { return this->torque; }

    inline float GetMass() const { return this->mass; }

    void ChangeMass(float mass, bool relative);

    inline float GetMomentOfInertia() const { return this->momentOfInertia; }

    float GetAngularMass(glm::vec2 r, glm::vec2 n) const;

protected:
    glm::vec2 position;
    glm::vec2 linearVelocity;
    glm::vec2 force;

    float angle;
    float angularVelocity;
    float torque;

    float mass;
    float momentOfInertia;

    ObjectType shapeType;
};

#endif //PHYSICALENGINE_OBJECT_H
