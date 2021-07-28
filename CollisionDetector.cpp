#include "CollisionDetector.h"
#include "Circle.h"
#include "Box2D.h"
#include <iostream>

bool CollisionDetector::ShapeContainsPoint(Object *object, glm::vec2 v) {
    return object->ContainsPoint(v);
}

CollisionInfo CollisionDetector::Detect(Object *object1, Object *object2) {
    if (dynamic_cast<Circle *>(object1) != nullptr && dynamic_cast<Circle *>(object2) != nullptr) {
        return Circle::DetectCollision(dynamic_cast<Circle *>(object1), dynamic_cast<Circle *>(object2));
    } else if (dynamic_cast<Box2D *>(object1) != nullptr && dynamic_cast<Box2D *>(object2) != nullptr) {
        return Box2D::DetectCollision(dynamic_cast<Box2D *>(object1), dynamic_cast<Box2D *>(object2));
    } else if (dynamic_cast<Circle *>(object1) != nullptr && dynamic_cast<Box2D *>(object2) != nullptr) {
        return Detect(dynamic_cast<Circle *>(object1), dynamic_cast<Box2D *>(object2));
    } else if (dynamic_cast<Circle *>(object2) != nullptr && dynamic_cast<Box2D *>(object1) != nullptr) {
        return Detect(dynamic_cast<Circle *>(object2), dynamic_cast<Box2D *>(object1));
    }
    return CollisionInfo(
            {object1, object2, glm::vec2(0), glm::vec2(0), glm::vec2(0), CollisionType::BoxBox, 0, false});;
}

std::vector<CollisionInfo> CollisionDetector::Detect(std::vector<Object *> *objects) {
    static const float EPS = 1e-5;

    std::vector<CollisionInfo> result;

    if (objects->size() < 2) return result;
    for (int i = 0; i < objects->size() - 1; i++) {
        auto box1 = (*objects)[i];
        for (int j = i + 1; j < objects->size(); j++) {
            auto box2 = (*objects)[j];

            auto collision_info_1 = Detect(box1, box2);
            auto collision_info_2 = Detect(box2, box1);

            bool isInternal = collision_info_1.object1->GetShapeType() == ObjectType::Internal;

            if (collision_info_1.isCollided && (isInternal || collision_info_2.isCollided)) {
                auto pd1 = collision_info_1.penetrationDepth;
                auto pd2 = isInternal ? 1E6f : collision_info_2.penetrationDepth;

                if (glm::min(pd1, pd2) < EPS) continue;

                if (pd1 < pd2)
                    result.push_back(collision_info_1);
                else
                    result.push_back(collision_info_2);
            }
        }
    }
    return result;
}

CollisionInfo DetectInternalBoxCircle(Circle *circle, Box2D *box) {
    glm::vec2 contactPoint;
    glm::vec2 normal;
    bool collided;
    if (circle->GetPosition().x + circle->GetRadius() > box->GetVertices()[1].x) {
        contactPoint = glm::vec2(box->GetVertices()[1].x, circle->GetPosition().y);
        normal = glm::normalize(circle->GetPosition() - contactPoint);
        collided = true;
    } else if (circle->GetPosition().x - circle->GetRadius() < box->GetVertices()[3].x) {
        contactPoint = glm::vec2(box->GetVertices()[3].x, circle->GetPosition().y);
        normal = glm::normalize(circle->GetPosition() - contactPoint);
        collided = true;
    } else if (circle->GetPosition().y + circle->GetRadius() > box->GetVertices()[3].y) {
        contactPoint = glm::vec2(circle->GetPosition().x, box->GetVertices()[3].y);
        normal = glm::normalize(circle->GetPosition() - contactPoint);
        collided = true;
    } else if (circle->GetPosition().y - circle->GetRadius() < box->GetVertices()[1].y) {
        contactPoint = glm::vec2(circle->GetPosition().x, box->GetVertices()[1].y);
        normal = glm::normalize(circle->GetPosition() - contactPoint);
        collided = true;
    } else {
        collided = false;
    }
    float penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);
    return CollisionInfo(
            {circle, box, circle->GetPosition(), contactPoint, normal, CollisionType::BoxCircle, penetration,
             collided});
}

CollisionInfo CollisionDetector::Detect(Circle *circle, Box2D *box) {
    bool isInternal = box->GetShapeType() == ObjectType::Internal;
    if (isInternal)
        return DetectInternalBoxCircle(circle, box);

    glm::vec2 originalCircle = glm::vec2(cos(box->GetAngle()) * (circle->GetPosition().x - box->GetPosition().x) -
                                         sin(box->GetAngle()) * (circle->GetPosition().y - box->GetPosition().y) +
                                         box->GetPosition().x,
                                         sin(box->GetAngle()) * (circle->GetPosition().x - box->GetPosition().x) +
                                         cos(box->GetAngle()) * (circle->GetPosition().y - box->GetPosition().y) +
                                         box->GetPosition().y);

    glm::vec2 nearestPoint = glm::vec2(originalCircle.x, originalCircle.y);
    if (originalCircle.x < box->GetVertices()[0].x)
        nearestPoint.x = box->GetVertices()[0].x;
    else if (originalCircle.x > box->GetVertices()[0].x + box->GetWidth())
        nearestPoint.x = box->GetVertices()[0].x + box->GetWidth();

    if (originalCircle.y < box->GetVertices()[0].y)
        nearestPoint.y = box->GetVertices()[0].y;
    else if (originalCircle.y > box->GetVertices()[0].y + box->GetHeight())
        nearestPoint.y = box->GetVertices()[0].y + box->GetHeight();

    double distance = glm::distance(originalCircle, nearestPoint);
    auto collisionInfo = CollisionInfo(
            {circle, box, glm::vec2(0), nearestPoint,
             glm::normalize(circle->GetPosition() - nearestPoint),
             CollisionType::BoxCircle,
             circle->GetRadius() - glm::length(circle->GetPosition() - nearestPoint), false});

    if (distance < circle->GetRadius()) {
        collisionInfo.penetrationPoint = circle->GetCenter() -
                                         glm::normalize(circle->GetPosition() - nearestPoint) *
                                         circle->GetRadius();
        collisionInfo.isCollided = true;
    }
    return collisionInfo;
}