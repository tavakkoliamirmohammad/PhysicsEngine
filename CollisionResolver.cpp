#include "CollisionResolver.h"

void CollisionResolver::Resolve(CollisionInfo &info) {
    if (info.isCollided) {
        if (info.collisionType == CollisionType::BoxBox) {
            ResolveAngular(info);
            auto v = info.contactPoint - info.penetrationPoint;
            auto r = info.object2->GetMass() / (info.object1->GetMass() + info.object2->GetMass());
            info.object1->MoveTo(-v.x * r, -v.y * r, true);
            info.object2->MoveTo(v.x * (1 - r), v.y * (1 - r), true);
        } else if (info.collisionType == CollisionType::CircleCircle) {
            auto n = info.normal;
            auto p = info.penetrationDepth;
            auto r = info.object1->GetMass() / (info.object1->GetMass() + info.object2->GetMass());
            info.object1->MoveTo(n.x * r * p, n.y * r * p, true);
            info.object2->MoveTo(-n.x * (1 - r) * p, -n.y * (1 - r) * p, true);
        } else if (info.collisionType == CollisionType::BoxCircle) {
            auto n = info.normal;
            auto p = info.penetrationDepth;
            auto r = info.object2->GetMass() / (info.object1->GetMass() + info.object2->GetMass());
            info.object1->MoveTo(n.x * r * p, n.y * r * p, true);
            info.object2->MoveTo(-n.x * (1 - r) * p, -n.y * (1 - r) * p, true);
        }
    }
}

void CollisionResolver::Resolve(std::vector<CollisionInfo> &info) {
    for (auto collisionInfo : info) {
        Resolve(collisionInfo);
    }
}

float angleBetween(glm::vec2 a, glm::vec2 b) {
    glm::vec2 da = glm::normalize(a);
    glm::vec2 db = glm::normalize(b);

    auto c = glm::cross(glm::vec3(a, 0), glm::vec3(b, 0)).z >= 0 ? -1 : 1;
    return c * glm::acos(glm::max(0.0f, glm::min(1.0f, glm::dot(da, db))));
}

void CollisionResolver::ResolveAngular(CollisionInfo &info) {
    auto c1 = info.object1->GetCentroid();
    auto c2 = info.object2->GetCentroid();

    auto r1 = info.penetrationPoint - c1;
    auto r2 = info.penetrationPoint - c2;

    auto p1 = info.contactPoint - c1;
    auto p2 = info.contactPoint - c2;

    auto alpha1 = angleBetween(r1, p1);
    auto alpha2 = angleBetween(r2, p2);

    if (info.object1->GetShapeType() != ObjectType::Internal) info.object1->Rotate(alpha1, true);
    if (info.object2->GetShapeType() != ObjectType::Internal) info.object2->Rotate(alpha2, true);
}