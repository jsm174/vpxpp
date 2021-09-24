#pragma once

#include "Vertex2D.h"

class FRect
{
public:
	Vertex2D Center() const;

	float left;
	float top;
	float right;
	float bottom;
};