#pragma once

#ifndef QUATERNION_H
#define QUATERNION_H


struct Quaternion
{
    Quaternion(float i, float j, float k, float r = 0.f);
    Quaternion();

    // Hamilton product
    Quaternion operator*(const Quaternion& other) const;

    // Inverse
    Quaternion operator~() const;

    union
    {
        float i;
        float x;
    };
    union
    {
        float j;
        float y;
    };
    union
    {
        float k;
        float z;
    };
    float r;
};

#endif //QUATERNION_H
