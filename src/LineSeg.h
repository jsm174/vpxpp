#pragma once

#include "Ball.h"
#include "CollisionEvent.h"
#include "HitObject.h"
#include "Vertex2D.h"

class LineSeg : public HitObject
{
public:
	LineSeg();
	LineSeg(const Vertex2D& p1, const Vertex2D& p2, const float zlow, const float zhigh);

	virtual float HitTest(const BallS& ball, const float dtime, CollisionEvent& coll) const;
	virtual int GetType() const;
	virtual void Collide(const CollisionEvent& coll);
	virtual void CalcHitBBox();

	float HitTestBasic(const BallS& ball, const float dtime, CollisionEvent& coll, const bool direction, const bool lateral, const bool rigid) const;
	void CalcNormal();

	Vertex2D normal;
	Vertex2D v1;
	Vertex2D v2;
	float length;
};