#include "FRect.h"

Vertex2D FRect::Center() const
{
	return Vertex2D(0.5f * (left + right), 0.5f * (top + bottom));
}