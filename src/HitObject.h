#pragma once

#include "FRect3D.h"
#include "IFireEvents.h"
#include "misc.h"

class Ball;

class HitObject
{
public:
	void FireHitEvent(Ball* const pball);

	IFireEvents* m_obj;

	float m_threshold;

	FRect3D m_hitBBox;

	float m_elasticity;
	float m_elasticityFalloff;
	float m_friction;
	float m_scatter;

	eObjType m_ObjType;

	bool m_enabled;

	bool m_fe;
	bool m_e;
};