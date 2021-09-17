#pragma once

class Bool4
{
public:
    Bool4();
    Bool4(const bool _x, const bool _y, const bool _z, const bool _w);

    union
    {
        struct {
            bool x;
            bool y;
            bool z;
            bool w;
        };
        unsigned int xyzw;
    };
};