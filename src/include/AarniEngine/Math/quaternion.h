#ifndef _QUATERNIONH_
#define _QUATERNIONH_
#include <math.h>

#include <AarniEngine/Math/vector.h>

//TODO
//eulerAngles. Figure out how to present angles with vector3, then program the function.

struct Quaternion
{
    float x, y, z, w = 0;
    Quaternion (float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {};
    public:

    // -------------------- Functions -------------------- //
    static Quaternion Conjugate(Quaternion q)
    {
        return {-q.x, -q.y, -q.z, q.w};
    }

    static float Magnitude(Quaternion q)
    {
        return sqrt(SquaredMagnitude(q));
    }

    static float SquaredMagnitude(Quaternion q)
    {
        return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    }

    static Quaternion Inverse(Quaternion q)
    {
        Quaternion c = Conjugate(q);
        float sm = SquaredMagnitude(q);
        return {c.x / sm, c.y / sm, c.z / sm, c.w / sm};
    }

    static Quaternion Sign(Quaternion q)
    {
        return q / Magnitude(q);
    }

    static float Argument(Quaternion q)
    {
        return acos(q.w / Magnitude(q));
    }

    // -------------------- Overriding operators -------------------- //
    
    // ----- plus ----- //
    Quaternion& operator+=(const Quaternion& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    friend Quaternion operator+(Quaternion q1, const Quaternion& q2)
    {
        return q1 += q2;
    }

    Quaternion operator+() const{
        return Quaternion(x, y, z, w);
    }

    // ----- Minus ----- //
    Quaternion& operator-=(const Quaternion& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    friend Quaternion operator-(Quaternion q1, const Quaternion& q2)
    {
        return q1 -= q2;
    }

    Quaternion operator-() const{
        return Quaternion(-x, -y, -z, -w);
    }

    // ----- Multiplication ----- //
    friend Quaternion operator*(Quaternion q, const float f)
    {
        return {q.x * f, q.y * f, q.z * f, q.w * f};
    }

    friend Quaternion operator*(Quaternion q1, Quaternion q2)
    {
        return {
            +q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
            -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
            +q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
            -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w,
        };
    }

    Quaternion& operator*=(const float& multiplier)
    {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        w *= multiplier;
        return *this;
    }

    // ----- Division ----- //
    friend Quaternion operator/(Quaternion q1, Quaternion q2)
    {
        return q1 * Inverse(q2);
    }

    Quaternion& operator/=(const Quaternion& q2)
    {
        Quaternion result = (Quaternion){x,y,z,w} * Inverse(q2);
        x = result.x;
        y = result.y;
        z = result.z;
        w = result.w;
        return *this;
    }
    
    friend Quaternion operator/(Quaternion q, float f)
    {
        return q / f;
    }

    Quaternion& operator/=(const float& f)
    {
        x /= f;
        y /= f;
        z /= f;
        w /= f;
        return *this;
    }

    // ----- Equal ----- //
    bool operator==(const Vector3& other) const{
        return x == other.x && y == other.y && z == other.z;
    }

    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& pt); //overriding << operator

    // -------------------- Defined values -------------------- //
    static Quaternion Zero() { return {0,0,0,0}; }
};

std::ostream& operator<<(std::ostream& os, const Quaternion& q) // overriding << operator
{
    os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return os;
}

#endif