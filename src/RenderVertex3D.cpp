#include "RenderVertex3D.h"

const int RenderVertex3D::Dim = 3;

void RenderVertex3D::set(const Vertex3Ds& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}