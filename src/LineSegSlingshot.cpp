#include "LineSegSlingshot.h"

#include "Surface.h"

#include "Player.h"
#include "extern.h"

LineSegSlingshot::LineSegSlingshot(const Vertex2D& p1, const Vertex2D& p2, const float _zlow, const float _zhigh)
	: LineSeg(p1, p2, _zlow, _zhigh)
{
	m_slingshotanim.m_iframe = false;
	m_slingshotanim.m_TimeReset = 0;
	m_doHitEvent = false;
	m_force = 0.f;
	m_EventTimeReset = 0;
	m_psurface = NULL;
}

int LineSegSlingshot::GetType() const
{
	return eLineSegSlingshot;
}

////////////////////////////////////////////////////////////////////////////////
//
// license:GPLv3+
// Ported at: VisualPinball.Unity/VisualPinball.Unity/Physics/Collider/LineCollider.cs
//

float LineSegSlingshot::HitTest(const BallS& ball, const float dtime, CollisionEvent& coll) const
{
	return HitTestBasic(ball, dtime, coll, true, true, true);
}

void LineSegSlingshot::Collide(const CollisionEvent& coll)
{
	Ball* const pball = coll.m_ball;
	const Vertex3Ds& hitnormal = coll.m_hitnormal;

	const float dot = coll.m_hitnormal.Dot(coll.m_ball->m_d.m_vel);

	const bool threshold = (dot <= -m_psurface->m_d.m_slingshot_threshold);

	if (!m_psurface->m_disabled && threshold)
	{
		const float len = (v2.x - v1.x) * hitnormal.y - (v2.y - v1.y) * hitnormal.x;
		const Vertex2D vhitpoint(pball->m_d.m_pos.x - hitnormal.x * pball->m_d.m_radius,
								 pball->m_d.m_pos.y - hitnormal.y * pball->m_d.m_radius);

		const float btd = (vhitpoint.x - v1.x) * hitnormal.y - (vhitpoint.y - v1.y) * hitnormal.x;
		float force = (fabsf(len) > 1.0e-6f) ? ((btd + btd) / len - 1.0f) : -1.0f;
		force = 0.5f * (1.0f - force * force);
		force *= m_force;

		pball->m_d.m_vel -= hitnormal * force;
	}

	pball->Collide3DWall(hitnormal, m_elasticity, m_elasticityFalloff, m_friction, m_scatter);

	if (m_obj && m_fe && !m_psurface->m_disabled && threshold)
	{
		const float dist_ls = (pball->m_lastEventPos - pball->m_d.m_pos).LengthSquared();
		pball->m_lastEventPos = pball->m_d.m_pos;

		if (dist_ls > 0.25f)
		{
			((IFireEvents*)m_obj)->FireGroupEvent(DISPID_SurfaceEvents_Slingshot);
			m_slingshotanim.m_TimeReset = g_pplayer->m_time_msec + 100;
		}
	}
}
