
#include "Quaternion.h"

Quaternion::Quaternion(float i, float j, float k, float r)
: i(i)
, j(j)
, k(k)
, r(r)
{
}

Quaternion::Quaternion()
: i(0.f)
, j(0.f)
, k(0.f)
, r(0.f)
{

}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
    return Quaternion
    (
        r * other.i + i * other.r + j * other.k - k * other.j,
        r * other.j - i * other.k + j * other.r + k * other.i,
        r * other.k + i * other.j - j * other.i + k * other.r,
        r * other.r - i * other.i - j * other.j - k * other.k
    );
}

Quaternion Quaternion::operator~() const
{
    return Quaternion(-i, -j, -k, r);
}
