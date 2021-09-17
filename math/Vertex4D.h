#pragma once

class Vertex4D
{
public:
	Vertex4D();
	Vertex4D(const float _x, const float _y, const float _z, const float _w);

	float x;
    float y;
    float z;
    float w;
};