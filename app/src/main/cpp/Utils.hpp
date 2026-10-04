```cpp
#pragma once
#include <cstdint>
#include <cmath>

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    float Length() const { return sqrtf(x*x + y*y + z*z); }
};

struct Vec2 { float x, y; };

template<typename T> inline T Read(uintptr_t a) {
    return a ? *reinterpret_cast<T*>(a) : T{};
}

template<typename T> inline void Write(uintptr_t a, T v) {
    if (a) *reinterpret_cast<T*>(a) = v;
}

inline Vec3 CalcAngle(const Vec3& s, const Vec3& d) {
    Vec3 delta = d - s;
    float hyp = sqrtf(delta.x*delta.x + delta.z*delta.z);
    return {
        -atan2f(delta.y, hyp) * 57.2957795f,
         atan2f(delta.z, delta.x) * 57.2957795f,
         0.0f
    };
}

inline Vec3 Smooth(const Vec3& cur, const Vec3& tgt, float sm) {
    Vec3 d = tgt - cur;
    while (d.x >  180.f) d.x -= 360.f;
    while (d.x < -180.f) d.x += 360.f;
    while (d.y >  180.f) d.y -= 360.f;
    while (d.y < -180.f) d.y += 360.f;
    d.x /= sm * 15.f;
    d.y /= sm * 15.f;
    return cur + d;
}
```
