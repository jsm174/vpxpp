#include "RenderVertex.h"

const int RenderVertex::Dim = 2;

void RenderVertex::set(const Vertex3Ds& v)
{
	x = v.x;
	y = v.y;
}

void RenderVertex::set(const RenderVertex& v)
{
	*this = v;
}

void RenderVertex::set(const RenderVertex3D& v)
{
	x = v.x;
	y = v.y;
	smooth = v.smooth;
	slingshot = v.slingshot;
	controlPoint = v.controlPoint;
}
