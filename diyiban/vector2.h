#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2
{
public:
    float x = 0;
    float y = 0;

public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(float x, float y)
        : x(x), y(y)
    {
    }
};

#endif
