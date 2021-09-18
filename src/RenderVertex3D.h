#pragma once

#include "Vertex3Ds.h"

class RenderVertex3D : public Vertex3Ds
{
public:
	static const int Dim;

	void set(const Vertex3Ds& v);

	bool smooth;
	bool slingshot;
	bool controlPoint;
	bool padd;
};