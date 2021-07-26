#include "CollisionDetector.h"
#include "Circle.h"
#include "Box2D.h"

bool CollisionDetector::ShapeContainsPoint(Object *object, glm::vec2 v) {
    return object->ContainsPoint(v);
}

CollisionInfo CollisionDetector::Detect(Object *object1, Object *object2) {
    if (dynamic_cast<Circle *>(object1) != nullptr && dynamic_cast<Circle *>(object2) != nullptr) {
        return Circle::DetectCollision(dynamic_cast<Circle *>(object1), dynamic_cast<Circle *>(object2));
    } else if (dynamic_cast<Box2D *>(object1) != nullptr && dynamic_cast<Box2D *>(object2) != nullptr) {
        return Box2D::DetectCollision(dynamic_cast<Box2D *>(object1), dynamic_cast<Box2D *>(object2));
    }
    return CollisionInfo({object1, object2, glm::vec2(0), glm::vec2(0), glm::vec2(0), CollisionType::BoxBox, 0, false});;
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