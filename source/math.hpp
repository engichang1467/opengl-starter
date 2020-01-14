#pragma once

#include <math.h>
#include <iostream>
#include "types.hpp"

// Scalar
#define PI 3.14159265359f;

f32 radians(f32 degrees)
{
    return degrees * 3.14159265359f / 180.0f;
}

// Declarations
template <typename T>
struct M2;

template <typename T>
struct M3;

template <typename T>
struct M4;

// Vector 2
template <typename T>
struct V2
{
    T x;
    T y;

    V2(){};
    V2(T x, T y) : x(x), y(y){};

    V2<T> operator+(T n)
    {
        return V2<T>(x + n, y + n);
    }
    V2<T> operator+(V2<T> v)
    {
        return V2<T>(x + v.x, y + v.y);
    }
    V2<T> operator-(T n)
    {
        return V2<T>(x - n, y - n);
    }
    V2<T> operator-(V2<T> v)
    {
        return V2<T>(x - v.x, y - v.y);
    }
    V2<T> operator*(T n)
    {
        return V2<T>(x * n, y * n);
    }
    V2<T> operator*(V2<T> v)
    {
        return V2<T>(x * v.x, y * v.y);
    }
    V2<T> operator*(M2<T> m)
    {
        return V2<T>(
            x * m.x.x + y * m.y.x,
            x * m.x.y + y * m.y.y);
    }
    V2<T> operator/(T n)
    {
        return V2<T>(x / n, y / n);
    }
};

template <typename T>
void out(V2<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << "]" << std::endl;
}

template <typename T>
T dot(V2<T> a, V2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
T length(V2<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

template <typename T>
V2<T> normalize(V2<T> v)
{
    return v / length(v);
}

// Vector 3
template <typename T>
struct V3
{
    T x;
    T y;
    T z;

    V3(){};
    V3(T x, T y, T z) : x(x), y(y), z(z){};

    V3<T> operator+(T n)
    {
        return V3<T>(x + n, y + n, z + n);
    }
    V3<T> operator+(V3<T> v)
    {
        return V3<T>(x + v.x, y + v.y, z + v.z);
    }
    V3<T> operator-(T n)
    {
        return V3<T>(x - n, y - n, z - n);
    }
    V3<T> operator-(V3<T> v)
    {
        return V3<T>(x - v.x, y - v.y, z - v.z);
    }
    V3<T> operator*(T n)
    {
        return V3<T>(x * n, y * n, z * n);
    }
    V3<T> operator*(V3<T> v)
    {
        return V3<T>(x * v.x, y * v.y, z * v.z);
    }
    V3<T> operator*(M3<T> m)
    {
        return V3<T>(
            x * m.x.x + y * m.y.x + z * m.z.x,
            x * m.x.y + y * m.y.y + z * m.z.y,
            x * m.x.z + y * m.y.z + z * m.z.z);
    }
    V3<T> operator/(T n)
    {
        return V3<T>(x / n, y / n, z / n);
    }
};

template <typename T>
void out(V3<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}

template <typename T>
T dot(V3<T> a, V3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T length(V3<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template <typename T>
V3<T> normalize(V3<T> v)
{
    return v / length(v);
}

template <typename T>
V3<T> cross(V3<T> a, V3<T> b)
{
    return V3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// Vector 4
template <typename T>
struct V4
{
    T x;
    T y;
    T z;
    T w;

    V4(){};
    V4(V3<T> v, T w) : x(v.x), y(v.y), z(v.z), w(w){};
    V4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w){};

    V4<T> operator+(T n)
    {
        return V4<T>(x + n, y + n, z + n, w + n);
    }
    V4<T> operator+(V4<T> v)
    {
        return V4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    V4<T> operator-(T n)
    {
        return V4<T>(x - n, y - n, z - n, w - n);
    }
    V4<T> operator-(V4<T> v)
    {
        return V4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    V4<T> operator*(T n)
    {
        return V4<T>(x * n, y * n, z * n, w * n);
    }
    V4<T> operator*(V4<T> v)
    {
        return V4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
    }
    V4<T> operator*(M4<T> m)
    {
        return V4<T>(
            x * m.x.x + y * m.y.x + z * m.z.x + w * m.w.x,
            x * m.x.y + y * m.y.y + z * m.z.y + w * m.w.y,
            x * m.x.z + y * m.y.z + z * m.z.z + w * m.w.z,
            x * m.x.w + y * m.y.w + z * m.z.w + w * m.w.w);
    }
    V4<T> operator/(T n)
    {
        return V4<T>(x / n, y / n, z / n, w / n);
    }
};

template <typename T>
void out(V4<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]" << std::endl;
}

template <typename T>
T dot(V4<T> a, V4<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
T length(V4<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template <typename T>
V4<T> normalize(V4<T> v)
{
    return v / length(v);
}

// Matrix 2
template <typename T>
struct M2
{
    V2<T> x;
    V2<T> y;

    M2(){};
    M2(T n) : x(V2(n, 0)), y(V2(0, n)){};
    M2(V2<T> x, V2<T> y) : x(x), y(y){};

    M2<T> operator*(M2<T> m)
    {
        return M2<T>(m.x * (*this), m.y * (*this));
    }
};

template <typename T>
void out(M2<T> m)
{
    out(m.x);
    out(m.y);
}

template <typename T>
M2<T> transpose(M2<T> m)
{
    return M2<T>(
        V2<T>(m.x.x, m.y.x),
        V2<T>(m.x.y, m.y.y));
}

// Matrix 3
template <typename T>
struct M3
{
    V3<T> x;
    V3<T> y;
    V3<T> z;

    M3(){};
    M3(T n) : x(V3(n, 0, 0)), y(V3(0, n, 0)), z(V3(0, 0, n)){};
    M3(V3<T> x, V3<T> y, V3<T> z) : x(x), y(y), z(z){};

    M3<T> operator*(M3<T> m)
    {
        return M3<T>(m.x * (*this), m.y * (*this), m.z * (*this));
    }
};

template <typename T>
void out(M3<T> m)
{
    out(m.x);
    out(m.y);
    out(m.z);
}

template <typename T>
M3<T> transpose(M3<T> m)
{
    return M3<T>(
        V3<T>(m.x.x, m.y.x, m.z.x),
        V3<T>(m.x.y, m.y.y, m.z.y),
        V3<T>(m.x.z, m.y.z, m.z.x));
}

// Matrix 4
template <typename T>
struct M4
{
    V4<T> x;
    V4<T> y;
    V4<T> z;
    V4<T> w;

    M4(){};
    M4(T n) : x(V4(n, 0, 0, 0)), y(V4(0, n, 0, 0)), z(V4(0, 0, n, 0)), w(V4(0, 0, 0, n)){};
    M4(V4<T> x, V4<T> y, V4<T> z, V4<T> w) : x(x), y(y), z(z), w(w){};

    M4<T> operator*(M4<T> m)
    {
        return M4<T>(m.x * (*this), m.y * (*this), m.z * (*this), m.w * (*this));
    }
};

template <typename T>
void out(M4<T> m)
{
    out(m.x);
    out(m.y);
    out(m.z);
    out(m.w);
}

template <typename T>
M4<T> transpose(M4<T> m)
{
    return M4<T>(
        V4<T>(m.x.x, m.y.x, m.z.x, m.w.x),
        V4<T>(m.x.y, m.y.y, m.z.y, m.w.y),
        V4<T>(m.x.z, m.y.z, m.z.x, m.w.z),
        V4<T>(m.x.w, m.y.w, m.z.w, m.w.w));
}

// Graphics Functions
template <typename T>
M4<T> lookAt(V3<T> position, V3<T> target, V3<T> worldUp)
{
    // 1. Position = known
    // 2. Calculate cameraDirection
    V3<T> zaxis = normalize(position - target);
    // 3. Get positive right axis vector
    V3<T> xaxis = normalize(cross(normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    V3<T> yaxis = cross(zaxis, xaxis);

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    M4<T> translation = {
        V4<T>(1, 0, 0, 0),
        V4<T>(0, 1, 0, 0),
        V4<T>(0, 0, 1, 0),
        V4<T>(position * -1, 1)};

    M4<T> rotation = {
        V4<T>(xaxis, 0),
        V4<T>(yaxis, 0),
        V4<T>(zaxis, 0),
        V4<T>(0, 0, 0, 1)};

    // Return lookAt matrix as combination of translation and rotation matrix
    return transpose(rotation) * translation;  // Remember to read from right to left (first translation then rotation)
}
