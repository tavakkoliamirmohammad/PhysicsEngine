#pragma once

#include "Box2D.h"
#include "PhysicalEngine.h"

class Scene
{
public:
	Scene() : selectedRigidBody(-1), physicalEngine(false)
	{ }

	void init();
	void update(float dt);
	void render();

	int AddBox(int h, int w, int mass, ObjectType type = ObjectType::External);
	void Select(int x, int y);
	void Select(int index);
	int GetSelectedRigidBody();
	void ChangeMass(float mass, bool relative = true);
	void Rotate(float angle, bool relative = true);
	void Move(float x, float y, bool isRelative = false);
	std::vector<Box2D*> GetRigidBodies();

private:

	PhysicalEngine physicalEngine;
	std::vector<Box2D*> rigidBodies;
	int selectedRigidBody;
};