#include "CollisionDetector.h"

bool CollisionDetector::ShapeContainsPoint(Box2D* box1, glm::vec2 v)
{
	auto box1_vertices = box1->GetVertices();
	auto n1 = box1_vertices.size();

	for (int i = 0; i < n1; i++)
	{
		auto a = box1_vertices[i];
		auto b = box1_vertices[(i + 1) % n1];

		auto n = GetEdgeNormal(a - b);

		bool isShapeInFrontOfEdge = true;

		if (glm::dot(v - a, n) > 0)
			return false;
	}

	return true;
}

glm::vec2 CollisionDetector::ProjectToEdge(glm::vec2 v1, glm::vec2 v2, glm::vec2 p)
{
	auto e1 = v2 - v1;
	auto e2 = p - v1;
	float valDp = glm::dot(e1, e2);
	float lenE1 = glm::length(e1);
	float lenE2 = glm::length(e2);
	float cos = valDp / (lenE1 * lenE2);
	float projLenOfLine = cos * lenE2;
	glm::vec2 projPoint = v1 + e1 / lenE1 * projLenOfLine;
	return projPoint;
}

glm::vec2 CollisionDetector::GetEdgeNormal(glm::vec2 edge)
{
	edge /= glm::length(edge);
	auto n = glm::cross(glm::vec3(0, 0, 1), glm::vec3(edge, 0));
	return glm::vec2(n.x, n.y);
}

bool CollisionDetector::IsPointBetweenTwoPoint(glm::vec2 v1, glm::vec2 v2, glm::vec2 p)
{
	auto e1 = v2 - v1;
	auto e2 = p - v1;
	auto e3 = p - v2;

	return glm::dot(e1, e2) > 0 && glm::dot(e1, e3) < 0;
}

CollisionInfo CollisionDetector::Detect(Box2D* box1, Box2D* box2)
{
	CollisionInfo info({ box1, box2, glm::vec2(0), glm::vec2(0), glm::vec2(0), 0, false });

	auto box1_vertices = box1->GetVertices();
	auto box2_vertices = box2->GetVertices();

	auto n1 = box1_vertices.size();
	auto n2 = box2_vertices.size();

	float minPenetration = 1e10;
	bool isInternal = box1->GetShapeType() == ShapeType::Internal;

	bool isShapeInFrontOfEdge = true;

	for (int i = 0; i < n1; i++)
	{
		auto a = box1_vertices[i];
		auto b = box1_vertices[(i + 1) % n1];

		auto n = GetEdgeNormal(a - b);
		if (isInternal) n = -n;
		else isShapeInFrontOfEdge = true;

		float maxPenetration = 0;
		CollisionInfo max_info({ box1, box2, glm::vec2(0), glm::vec2(0), glm::vec2(0), 0, false });

		for (int j = 0; j < n2; j++)
		{
			auto v = box2_vertices[j];

			if (glm::dot(v - a, n) < 0)
			{
				isShapeInFrontOfEdge = false;

				auto p = ProjectToEdge(a, b, v);
				//if (IsPointBetweenTwoPoint(a, b, p))
				{
					auto penetration = glm::length(p - v);

					if (penetration > maxPenetration)
					{
						maxPenetration = penetration;
						max_info = CollisionInfo({ box1, box2, v, p, n, penetration, true });
					}
				}
			}
		}

		if (isShapeInFrontOfEdge && !isInternal)
		{
			info.isCollided = false;
			return info;
		}
		else if (max_info.isCollided)
		{
			if (maxPenetration < minPenetration)
			{
				minPenetration = maxPenetration;
				info = max_info;
			}
		}
	}

	if (isShapeInFrontOfEdge) info.isCollided = false;
	return info;
}

std::vector<CollisionInfo> CollisionDetector::Detect(std::vector<Box2D*>* boxes)
{
	static const float EPS = 1e-5;

	std::vector<CollisionInfo> result;

	if (boxes->size() < 2) return result;
	for (int i = 0; i < boxes->size() - 1; i++)
	{
		auto box1 = (*boxes)[i];
		for (int j = i + 1; j < boxes->size(); j++)
		{
			auto box2 = (*boxes)[j];

			auto collision_info_1 = Detect(box1, box2);
			auto collision_info_2 = Detect(box2, box1);

			bool isInternal = collision_info_1.box1->GetShapeType() == ShapeType::Internal;

			if (collision_info_1.isCollided && (isInternal || collision_info_2.isCollided))
			{
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
