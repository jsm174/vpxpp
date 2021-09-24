#pragma once

class FRect3D
{
public:
	FRect3D();
	FRect3D(const float x1, const float x2, const float y1, const float y2, const float z1, const float z2);

	void Clear();
	void Extend(const FRect3D& other);

	float left;
	float right;
	float top;
	float bottom;
	float zlow;
	float zhigh;
};