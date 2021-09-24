#include "LineSeg.h"

#include "Inlines.h"
#include "physconst.h"

LineSeg::LineSeg()
{
}

LineSeg::LineSeg(const Vertex2D& p1, const Vertex2D& p2, const float zlow, const float zhigh)
	: v1(p1), v2(p2)
{
	m_hitBBox.zlow = zlow;
	m_hitBBox.zhigh = zhigh;
	CalcNormal();
}

int LineSeg::GetType() const
{
	return eLineSeg;
}

//
// license:GPLv3+
// Ported at: VisualPinball.Unity/VisualPinball.Unity/Physics/Collider/LineCollider.cs
//

float LineSeg::HitTestBasic(const BallS& ball, const float dtime, CollisionEvent& coll, const bool direction, const bool lateral, const bool rigid) const
{
	if (!m_enabled || ball.m_frozen)
	{
		return -1.0f;
	}

	const float ballvx = ball.m_vel.x;
	const float ballvy = ball.m_vel.y;

	const float bnv = ballvx * normal.x + ballvy * normal.y;
	bool bUnHit = (bnv > C_LOWNORMVEL);

	if (direction && bUnHit)
	{
		return -1.0f;
	}

	const float ballx = ball.m_pos.x;
	const float bally = ball.m_pos.y;

	const float rollingRadius = lateral ? ball.m_radius : C_TOL_RADIUS;
	const float bcpd = (ballx - v1.x) * normal.x + (bally - v1.y) * normal.y;
	float bnd = bcpd - rollingRadius;

	if (m_ObjType == eSpinner || m_ObjType == eGate)
	{
		bnd = bcpd + rollingRadius;
	}

	const bool inside = (bnd <= 0.f);

	float hittime;
	if (rigid)
	{
		if ((bnd < -ball.m_radius) || (lateral && bcpd < 0.f))
		{
			return -1.0f;
		}

		if (lateral && (bnd <= (float)PHYS_TOUCH))
		{
			if (inside || (fabsf(bnv) > C_CONTACTVEL) || (bnd <= (float)(-PHYS_TOUCH)))
			{
				hittime = 0;
			}
			else
			{
				hittime = bnd
#ifdef NEW_PHYSICS
						  / -bnv;
#else
							  * (float)(1.0 / (2.0 * PHYS_TOUCH)) +
						  0.5f;
#endif
			}
		}
		else if (fabsf(bnv) > C_LOWNORMVEL)
		{
			hittime = bnd / -bnv;
		}
		else
		{
			return -1.0f;
		}
	}
	else
	{
		if (bnv * bnd >= 0.f)
		{
			if ((m_ObjType != eTrigger) ||
				(!ball.m_vpVolObjs) ||
				(fabsf(bnd) >= ball.m_radius * 0.5f) ||
				(inside != (FindIndexOf(*(ball.m_vpVolObjs), m_obj) < 0)))
			{
				return -1.0f;
			}
			hittime = 0;
			bUnHit = !inside;
		}
		else
		{
			hittime = bnd / -bnv;
		}
	}

	if (infNaN(hittime) || hittime < 0.f || hittime > dtime)
	{
		return -1.0f;
	}
	const float btv = ballvx * normal.y - ballvy * normal.x;
	const float btd = (ballx - v1.x) * normal.y - (bally - v1.y) * normal.x + btv * hittime;

	if (btd < -C_TOL_ENDPNTS || btd > length + C_TOL_ENDPNTS)
	{
		return -1.0f;
	}
	if (!rigid)
	{
		coll.m_hitflag = bUnHit;
	}

	const float hitz = ball.m_pos.z + ball.m_vel.z * hittime;

	if (hitz + ball.m_radius * 0.5f < m_hitBBox.zlow || hitz - ball.m_radius * 0.5f > m_hitBBox.zhigh)
	{
		return -1.0f;
	}
	coll.m_hitnormal.x = normal.x;
	coll.m_hitnormal.y = normal.y;
	coll.m_hitnormal.z = 0.0f;

	coll.m_hitdistance = bnd;

	coll.m_isContact = (fabsf(bnv) <= C_CONTACTVEL && fabsf(bnd) <= (float)PHYS_TOUCH);
	if (coll.m_isContact)
	{
		coll.m_hit_org_normalvelocity = bnv;
	}

	return hittime;
}

float LineSeg::HitTest(const BallS& ball, const float dtime, CollisionEvent& coll) const
{
	return HitTestBasic(ball, dtime, coll, true, true, true);
}

void LineSeg::CalcNormal()
{
	const Vertex2D vT(v1.x - v2.x, v1.y - v2.y);

	length = vT.Length();
	const float inv_length = 1.0f / length;
	normal.x = vT.y * inv_length;
	normal.y = -vT.x * inv_length;
}

void LineSeg::CalcHitBBox()
{
	m_hitBBox.left = min(v1.x, v2.x);
	m_hitBBox.right = max(v1.x, v2.x);
	m_hitBBox.top = min(v1.y, v2.y);
	m_hitBBox.bottom = max(v1.y, v2.y);
}

void LineSeg::Collide(const CollisionEvent& coll)
{
	const float dot = coll.m_hitnormal.Dot(coll.m_ball->m_d.m_vel);
	coll.m_ball->Collide3DWall(coll.m_hitnormal, m_elasticity, m_elasticityFalloff, m_friction, m_scatter);

	if (dot <= -m_threshold)
	{
		FireHitEvent(coll.m_ball);
	}
}
