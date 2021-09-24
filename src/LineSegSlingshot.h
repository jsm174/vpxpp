#pragma once

#include "LineSeg.h"
#include "SlingshotAnimObject.h"

class Surface;

class LineSegSlingshot : public LineSeg
{
public:
	LineSegSlingshot(const Vertex2D& p1, const Vertex2D& p2, const float _zlow, const float _zhigh);

	virtual float HitTest(const BallS& ball, const float dtime, CollisionEvent& coll) const;
	virtual int GetType() const;
	virtual void Collide(const CollisionEvent& coll);

	Surface* m_psurface;

	float m_force;
	unsigned int m_EventTimeReset;

	SlingshotAnimObject m_slingshotanim;

	bool m_doHitEvent;
};