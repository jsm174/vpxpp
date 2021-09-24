#pragma once

#include "HitObject.h"
#include "IFireEvents.h"
#include "Vertex3Ds.h"

#include <vector>

struct BallS
{
	std::vector<IFireEvents*>* m_vpVolObjs;
	Vertex3Ds m_pos;
	Vertex3Ds m_vel;
	float m_radius;
	float m_mass;
	bool m_frozen;
};

class Ball : public HitObject
{
public:
	Ball();

	void Collide3DWall(const Vertex3Ds& hitNormal, float elasticity, const float elastFalloff, const float friction, float scatter_angle);

	BallS m_d;

	Vertex3Ds m_lastEventPos;
};