#define GLEW_STATIC

#include <GL/glew.h>

#include "Scene.h"
#include "CollisionDetector.h"
#include "Box2D.h"

void Scene::init() {
    selectedRigidBody = -1;
    physicalEngine.SetResolverAcivity(true);

    AddBox(100, 150, 1e8, ObjectType::Internal);
}

void Scene::update(float dt) {
    physicalEngine.SetRigidBodies(&this->rigidBodies);
    physicalEngine.Update(dt);
}

void Scene::render() {
    glPushAttrib(GL_LINE_BIT);

    // Draw Origin
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-20, 0);
    glVertex2f(20, 0);
    glVertex2f(0, -20);
    glVertex2f(0, 20);
    glEnd();

    // Draw Box
    for (int i = 0; i < this->rigidBodies.size(); i++) {
        if (dynamic_cast<Box2D *>(this->rigidBodies[i]) != nullptr) {
            auto *box2D = dynamic_cast<Box2D *>(this->rigidBodies[i]);
            if (box2D->GetShapeType() == ObjectType::Internal || this->selectedRigidBody == i)
                glColor3f(0, 0, 1);
            else
                glColor3f(0, 1, 0);

            glLineWidth(glm::min(2.f, box2D->GetMass()));
            glBegin(GL_LINE_LOOP);
            for (auto v : box2D->GetVertices())
                glVertex2f(v.x, v.y);
            glEnd();
        } else if (dynamic_cast<Circle *>(this->rigidBodies[i]) != nullptr) {
            auto *circle = dynamic_cast<Circle *>(this->rigidBodies[i]);
            if (circle->GetShapeType() == ObjectType::Internal || this->selectedRigidBody == i)
                glColor3f(0, 0, 1);
            else
                glColor3f(0, 1, 0);
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j <= 300; j++) {
                double angle = 2 * M_PI * j / 300;
                glVertex2d(circle->GetCenter().x + circle->GetRadius() * cos(angle),
                           circle->GetCenter().y + circle->GetRadius() * sin(angle));
            }
            glEnd();
        }
    }

    // Draw Collision info
    auto collisionInfo = physicalEngine.CurrentCollisionInfo();
    for (auto info : collisionInfo) {
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex2f(info.penetrationPoint.x, info.penetrationPoint.y);
        glVertex2f(info.contactPoint.x, info.contactPoint.y);
        glEnd();
    }

    glPopAttrib();
}

int Scene::AddBox(int h, int w, int mass, ObjectType type) {
    Box2D *box2d = new Box2D(h, w, mass, type);
    this->rigidBodies.push_back(box2d);

    physicalEngine.SetChangedRigidBodies(true);

    return this->rigidBodies.size() - 1;
}

int Scene::AddCircle(int r, int mass) {
    this->rigidBodies.push_back(new Circle(r, mass));
    physicalEngine.SetChangedRigidBodies(true);
    return this->rigidBodies.size() - 1;
}

void Scene::Select(int x, int y) {
    auto p = glm::vec2(x, y);
    for (int i = 0; i < this->rigidBodies.size(); i++) {
        auto box = this->rigidBodies[i];
        if (box->GetShapeType() == ObjectType::Internal) continue;

        if (CollisionDetector::ShapeContainsPoint(box, p)) {
            selectedRigidBody = i;
            return;
        }
    }
}

void Scene::Select(int index) {
    selectedRigidBody = glm::min((int) this->rigidBodies.size() - 1, glm::max(0, index));
}

int Scene::GetSelectedRigidBody() {
    return selectedRigidBody;
}

void Scene::ChangeMass(float mass, bool relative) {
    if (selectedRigidBody >= 0) {
        this->rigidBodies[selectedRigidBody]->ChangeMass(mass, relative);
        physicalEngine.SetChangedRigidBodies(true);
    }
}

void Scene::Rotate(float angle, bool relative) {
    if (selectedRigidBody >= 0) {
        this->rigidBodies[selectedRigidBody]->Rotate(angle, relative);
        physicalEngine.SetChangedRigidBodies(true);
    }
}

void Scene::Move(float x, float y, bool isRelative) {
    if (selectedRigidBody >= 0) {
        this->rigidBodies[selectedRigidBody]->MoveTo(x, y, isRelative);
        physicalEngine.SetChangedRigidBodies(true);
    }
}

std::vector<Object *> Scene::GetRigidBodies() {
    return this->rigidBodies;
}
