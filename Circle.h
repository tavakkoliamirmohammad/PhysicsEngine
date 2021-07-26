#ifndef PHYSICALENGINE_CIRCLE_H
#define PHYSICALENGINE_CIRCLE_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.h"
#include "CollisionDetector.h"

class Circle : public Object {
public:

    Circle(float radius, float mass);

    float GetRadius() const { return this->radius; };

    void SetRadius(float radius);

    glm::vec2 GetCenter();

    glm::vec2 GetCentroid() override;

    void CalculateInertia() override;

    bool ContainsPoint(glm::vec2 v) override;

    static CollisionInfo DetectCollision(Circle *circle1, Circle *circle2);

private:
    float radius;
};

#endif //PHYSICALENGINE_CIRCLE_H
