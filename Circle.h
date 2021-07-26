#ifndef PHYSICALENGINE_CIRCLE_H
#define PHYSICALENGINE_CIRCLE_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shape.h"

class Circle : public Shape {
public:

    Circle(float r, float mass, glm::vec2 c);

    float GetRadius() const { return this->radius; };

    void SetRadius(float radius);

    const glm::vec2 &GetOrigin() const;

    void SetOrigin(const glm::vec2 &origin);

    glm::vec2 GetCentroid() override;

    void CalculateInertia() override;

private:
    float radius;
    glm::vec2 origin;
};

#endif //PHYSICALENGINE_CIRCLE_H
