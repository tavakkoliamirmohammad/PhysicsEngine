#ifndef PHYSICALENGINE_CIRCLE_H
#define PHYSICALENGINE_CIRCLE_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Object.h"

class Circle : public Object {
public:

    Circle(float radius, float mass);

    float GetRadius() const { return this->radius; };

    void SetRadius(float radius);

    glm::vec2 GetCenter();

    glm::vec2 GetCentroid() override;

    void CalculateInertia() override;

private:
    float radius;
};

#endif //PHYSICALENGINE_CIRCLE_H
