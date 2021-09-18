#pragma once

#include "Vertex2D.h"
#include "Vertex3Ds.h"
#include "RenderVertex3D.h"

class RenderVertex : public Vertex2D
{
public:
	static const int Dim;

	void set(const Vertex3Ds& v);
	void set(const RenderVertex& v);
	void set(const RenderVertex3D& v);

	bool smooth;
	bool slingshot;
	bool controlPoint;
	bool padd;
};