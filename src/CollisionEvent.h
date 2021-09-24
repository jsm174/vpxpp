#pragma once

// Ported at: VisualPinball.Engine/Physics/CollisionEvent.cs

struct CollisionEvent
{
	CollisionEvent() : m_ball(0), m_obj(0), m_hittime(0.0f), m_hitdistance(0.0f), m_hitmoment_bit(true), m_hitflag(false), m_isContact(false) {}

	Ball* m_ball;
	HitObject* m_obj;

	float m_hittime;
	float m_hitdistance;

	Vertex3Ds m_hitnormal;
	Vertex2D m_hitvel;
	float m_hit_org_normalvelocity;

	bool m_hitmoment_bit;

	bool m_hitflag;

	bool m_isContact;
};