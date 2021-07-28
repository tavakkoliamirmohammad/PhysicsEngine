#include "CollisionDetector.h"
#include "Circle.h"
#include "Box2D.h"
#include <iostream>

bool CollisionDetector::ShapeContainsPoint(Object *object, glm::vec2 v) {
    return object->ContainsPoint(v);
}

double findDistance(double fromX, double fromY, double toX, double toY) {
    double a = abs(fromX - toX);
    double b = abs(fromY - toY);

    return sqrt((a * a) + (b * b));
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

CollisionInfo CollisionDetector::Detect(Circle *circle, Box2D *box) {
    bool isInternal = box->GetShapeType() == ObjectType::Internal;

    if (isInternal) {
        glm::vec2 normalVec;
        float penetration;
        glm::vec2 contactPoint;

        if (circle->GetPosition().x + circle->GetRadius() > box->GetVertices()[1].x) {
            contactPoint = glm::vec2(box->GetVertices()[1].x, circle->GetPosition().y);
            glm::vec2 normalVec = circle->GetPosition() - contactPoint;
            normalVec = glm::normalize(normalVec);
            penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);

            return CollisionInfo(
                    {circle, box, circle->GetPosition(), contactPoint, normalVec, CollisionType::BoxCircle, penetration,
                     true});
        } else if (circle->GetPosition().x - circle->GetRadius() < box->GetVertices()[3].x) {
            contactPoint = glm::vec2(box->GetVertices()[3].x, circle->GetPosition().y);
            glm::vec2 normalVec = circle->GetPosition() - contactPoint;
            normalVec = glm::normalize(normalVec);
            penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);

            return CollisionInfo(
                    {circle, box, circle->GetPosition(), contactPoint, normalVec, CollisionType::BoxCircle,
                     penetration, true});
        }
        if (circle->GetPosition().y + circle->GetRadius() > box->GetVertices()[3].y) {
            contactPoint = glm::vec2(circle->GetPosition().x, box->GetVertices()[3].y);
            glm::vec2 normalVec = circle->GetPosition() - contactPoint;
            normalVec = glm::normalize(normalVec);
            penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);

            return CollisionInfo(
                    {circle, box, circle->GetPosition(), contactPoint, normalVec, CollisionType::BoxCircle, penetration,
                     true});
        } else if (circle->GetPosition().y - circle->GetRadius() < box->GetVertices()[1].y) {
            contactPoint = glm::vec2(circle->GetPosition().x, box->GetVertices()[1].y);
            glm::vec2 normalVec = circle->GetPosition() - contactPoint;
            normalVec = glm::normalize(normalVec);
            penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);

            return CollisionInfo(
                    {circle, box, circle->GetPosition(), contactPoint, normalVec, CollisionType::BoxCircle, penetration,
                     true});
        } else {
            return CollisionInfo(
                    {circle, box, glm::vec2(0), contactPoint, normalVec, CollisionType::BoxCircle, penetration,
                     false});
        }
    }

    // Rotate circle's center point back
    double unrotatedCircleX = cos(box->GetAngle()) * (circle->GetPosition().x - box->GetPosition().x) -
                              sin(box->GetAngle()) * (circle->GetPosition().y - box->GetPosition().y) +
                              box->GetPosition().x;
    double unrotatedCircleY = sin(box->GetAngle()) * (circle->GetPosition().x - box->GetPosition().x) +
                              cos(box->GetAngle()) * (circle->GetPosition().y - box->GetPosition().y) +
                              box->GetPosition().y;

    // Closest point in the rectangle to the center of circle rotated backwards(unrotated)
    double closestX, closestY;

    // Find the unrotated closest x point from center of unrotated circle
    if (unrotatedCircleX < box->GetVertices()[0].x)
        closestX = box->GetVertices()[0].x;
    else if (unrotatedCircleX > box->GetVertices()[0].x + box->GetWidth())
        closestX = box->GetVertices()[0].x + box->GetWidth();
    else
        closestX = unrotatedCircleX;

    // Find the unrotated closest y point from center of unrotated circle
    if (unrotatedCircleY < box->GetVertices()[0].y)
        closestY = box->GetVertices()[0].y;
    else if (unrotatedCircleY > box->GetVertices()[0].y + box->GetHeight())
        closestY = box->GetVertices()[0].y + box->GetHeight();
    else
        closestY = unrotatedCircleY;

    // Determine collision
    bool collision = false;

    double distance = findDistance(unrotatedCircleX, unrotatedCircleY, closestX, closestY);

    auto contactPoint = glm::vec2(closestX, closestY);

    float penetration = circle->GetRadius() - glm::length(circle->GetPosition() - contactPoint);

    glm::vec2 normalVec = circle->GetPosition() - contactPoint;
    normalVec = glm::normalize(normalVec);

    if (distance < circle->GetRadius()) {
        collision = true;

        return CollisionInfo(
                {circle, box, circle->GetCenter() - normalVec * circle->GetRadius(), contactPoint, normalVec,
                 CollisionType::BoxCircle, penetration, true});
    } else {
        collision = false;
        return CollisionInfo(
                {circle, box, glm::vec2(0), contactPoint, normalVec, CollisionType::BoxCircle, penetration, false});
    }
}