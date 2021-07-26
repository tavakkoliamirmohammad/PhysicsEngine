#pragma once

#include "Box2D.h"
#include <tuple>

struct CollisionInfo
{
	Box2D* box1;
	Box2D* box2;
	glm::vec2 penetrationPoint;
	glm::vec2 contactPoint;
	glm::vec2 normal;
	float penetrationDepth;
	bool isCollided;
};

class CollisionDetector
{
public:
	static bool ShapeContainsPoint(Box2D* box1, glm::vec2 p);
	static glm::vec2 ProjectToEdge(glm::vec2 v1, glm::vec2 v2, glm::vec2 p);
	static glm::vec2 GetEdgeNormal(glm::vec2 edge);
	static bool IsPointBetweenTwoPoint(glm::vec2 v1, glm::vec2 v2, glm::vec2 p);
	CollisionInfo Detect(Box2D* box1, Box2D* box2);
	std::vector<CollisionInfo> Detect(std::vector<Box2D*>* boxes);
};