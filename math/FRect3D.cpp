#include "FRect3D.h"

#include "Inlines.h"
#include <float.h>

FRect3D::FRect3D()
{
}

FRect3D::FRect3D(const float x1, const float x2, const float y1, const float y2, const float z1, const float z2)
	: left(x1), right(x2),
	  top(y1), bottom(y2),
	  zlow(z1), zhigh(z2)
{
}

void FRect3D::Clear()
{
	left = FLT_MAX;
	right = -FLT_MAX;
	top = FLT_MAX;
	bottom = -FLT_MAX;
	zlow = FLT_MAX;
	zhigh = -FLT_MAX;
}

void FRect3D::Extend(const FRect3D& other)
{
	left = min(left, other.left);
	right = max(right, other.right);
	top = min(top, other.top);
	bottom = max(bottom, other.bottom);
	zlow = min(zlow, other.zlow);
	zhigh = max(zhigh, other.zhigh);
}