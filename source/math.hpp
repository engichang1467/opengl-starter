#pragma once

#include <math.h>
#include <iostream>
#include "types.hpp"

// change this typedef to change default type
typedef float F;

template <typename T>
struct M2_T;

template <typename T>
struct M3_T;

template <typename T>
struct M4_T;

// Scalar
const F32 PI = 3.14159265359f;

template <typename T>
F radians(T degrees)
{
    return (F)degrees * PI / 180.0f;
}

// Vector 2
template <typename T>
struct V2_T
{
    T x;
    T y;

    V2_T() : x(0), y(0){};
    V2_T(T x, T y) : x(x), y(y){};

    V2_T<T> operator+(T n)
    {
        return V2_T<T>(x + n, y + n);
    }
    V2_T<T> operator+(V2_T<T> v)
    {
        return V2_T<T>(x + v.x, y + v.y);
    }
    V2_T<T> operator-(T n)
    {
        return V2_T<T>(x - n, y - n);
    }
    V2_T<T> operator-(V2_T<T> v)
    {
        return V2_T<T>(x - v.x, y - v.y);
    }
    V2_T<T> operator*(T n)
    {
        return V2_T<T>(x * n, y * n);
    }
    V2_T<T> operator*(V2_T<T> v)
    {
        return V2_T<T>(x * v.x, y * v.y);
    }
    V2_T<T> operator*(M2_T<T> m)
    {
        return V2_T<T>(
            x * m.x.x + y * m.y.x,
            x * m.x.y + y * m.y.y);
    }
    V2_T<T> operator/(T n)
    {
        return V2_T<T>(x / n, y / n);
    }

    T* front()
    {
        return &x;
    }
};

template <typename T>
void out(V2_T<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << "]" << std::endl;
}

template <typename T>
T dot(V2_T<T> a, V2_T<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
T length(V2_T<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

template <typename T>
V2_T<T> normalize(V2_T<T> v)
{
    return v / length(v);
}

// Vector 3
template <typename T>
struct V3_T
{
    T x;
    T y;
    T z;

    V3_T() : x(0), y(0), z(0){};
    V3_T(T x, T y, T z) : x(x), y(y), z(z){};

    V3_T<T> operator+(T n)
    {
        return V3_T<T>(x + n, y + n, z + n);
    }
    V3_T<T> operator+(V3_T<T> v)
    {
        return V3_T<T>(x + v.x, y + v.y, z + v.z);
    }
    V3_T<T> operator-(T n)
    {
        return V3_T<T>(x - n, y - n, z - n);
    }
    V3_T<T> operator-(V3_T<T> v)
    {
        return V3_T<T>(x - v.x, y - v.y, z - v.z);
    }
    V3_T<T> operator*(T n)
    {
        return V3_T<T>(x * n, y * n, z * n);
    }
    V3_T<T> operator*(V3_T<T> v)
    {
        return V3_T<T>(x * v.x, y * v.y, z * v.z);
    }
    V3_T<T> operator*(M3_T<T> m)
    {
        return V3_T<T>(
            x * m.x.x + y * m.y.x + z * m.z.x,
            x * m.x.y + y * m.y.y + z * m.z.y,
            x * m.x.z + y * m.y.z + z * m.z.z);
    }
    V3_T<T> operator/(T n)
    {
        return V3_T<T>(x / n, y / n, z / n);
    }
    bool operator==(V3_T<T> v)
    {
        return x == v.x && y == v.y && z == v.z;
    }

    T* front()
    {
        return &x;
    }

    std::string string()
    {
        return "[" + std::to_string(x).substr(0, 8) + ", " + std::to_string(y).substr(0, 8) + ", " + std::to_string(z).substr(0, 8) + "]";
    }

    T hash()
    {
        return x * 23 + y * 29 + z * 31;
    }
};

template <typename T>
void out(V3_T<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}

template <typename T>
T dot(V3_T<T> a, V3_T<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T length(V3_T<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template <typename T>
V3_T<T> normalize(V3_T<T> v)
{
    return v / length(v);
}

template <typename T>
V3_T<T> cross(V3_T<T> a, V3_T<T> b)
{
    return V3_T<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// Vector 4
template <typename T>
struct V4_T
{
    T x;
    T y;
    T z;
    T w;

    V4_T() : x(0), y(0), z(0), w(0){};
    V4_T(V3_T<T> v, T w) : x(v.x), y(v.y), z(v.z), w(w){};
    V4_T(T x, T y, T z, T w) : x(x), y(y), z(z), w(w){};

    V4_T<T> operator+(T n)
    {
        return V4_T<T>(x + n, y + n, z + n, w + n);
    }
    V4_T<T> operator+(V4_T<T> v)
    {
        return V4_T<T>(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    V4_T<T> operator-(T n)
    {
        return V4_T<T>(x - n, y - n, z - n, w - n);
    }
    V4_T<T> operator-(V4_T<T> v)
    {
        return V4_T<T>(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    V4_T<T> operator*(T n)
    {
        return V4_T<T>(x * n, y * n, z * n, w * n);
    }
    V4_T<T> operator*(V4_T<T> v)
    {
        return V4_T<T>(x * v.x, y * v.y, z * v.z, w * v.w);
    }
    V4_T<T> operator*(M4_T<T> m)
    {
        return V4_T<T>(
            x * m.x.x + y * m.y.x + z * m.z.x + w * m.w.x,
            x * m.x.y + y * m.y.y + z * m.z.y + w * m.w.y,
            x * m.x.z + y * m.y.z + z * m.z.z + w * m.w.z,
            x * m.x.w + y * m.y.w + z * m.z.w + w * m.w.w);
    }
    V4_T<T> operator/(T n)
    {
        return V4_T<T>(x / n, y / n, z / n, w / n);
    }

    T* front()
    {
        return &x;
    }
};

template <typename T>
void out(V4_T<T> v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]" << std::endl;
}

template <typename T>
T dot(V4_T<T> a, V4_T<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
T length(V4_T<T> v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template <typename T>
V4_T<T> normalize(V4_T<T> v)
{
    return v / length(v);
}

// Matrix 2
template <typename T>
struct M2_T
{
    V2_T<T> x;
    V2_T<T> y;

    M2_T(){};
    M2_T(T n) : x(V2_T<T>(n, 0)), y(V2_T<T>(0, n)){};
    M2_T(V2_T<T> x, V2_T<T> y) : x(x), y(y){};

    M2_T<T> operator*(M2_T<T> m)
    {
        return M2_T<T>(m.x * (*this), m.y * (*this));
    }

    T* front()
    {
        return &x.x;
    }
};

template <typename T>
void out(M2_T<T> m)
{
    out(m.x);
    out(m.y);
}

template <typename T>
M2_T<T> transpose(M2_T<T> m)
{
    return M2_T<T>(
        V2_T<T>(m.x.x, m.y.x),
        V2_T<T>(m.x.y, m.y.y));
}

// Matrix 3
template <typename T>
struct M3_T
{
    V3_T<T> x;
    V3_T<T> y;
    V3_T<T> z;

    M3_T(){};
    M3_T(T n) : x(V3_T<T>(n, 0, 0)), y(V3_T<T>(0, n, 0)), z(V3_T<T>(0, 0, n)){};
    M3_T(V3_T<T> x, V3_T<T> y, V3_T<T> z) : x(x), y(y), z(z){};

    M3_T<T> operator*(M3_T<T> m)
    {
        return M3_T<T>(m.x * (*this), m.y * (*this), m.z * (*this));
    }

    T* front()
    {
        return &x.x;
    }
};

template <typename T>
void out(M3_T<T> m)
{
    out(m.x);
    out(m.y);
    out(m.z);
}

template <typename T>
M3_T<T> transpose(M3_T<T> m)
{
    return M3_T<T>(
        V3_T<T>(m.x.x, m.y.x, m.z.x),
        V3_T<T>(m.x.y, m.y.y, m.z.y),
        V3_T<T>(m.x.z, m.y.z, m.z.x));
}

// Matrix 4
template <typename T>
struct M4_T
{
    V4_T<T> x;
    V4_T<T> y;
    V4_T<T> z;
    V4_T<T> w;

    M4_T(){};
    M4_T(T n) : x(V4_T<T>(n, 0, 0, 0)), y(V4_T<T>(0, n, 0, 0)), z(V4_T<T>(0, 0, n, 0)), w(V4_T<T>(0, 0, 0, n)){};
    M4_T(V4_T<T> x, V4_T<T> y, V4_T<T> z, V4_T<T> w) : x(x), y(y), z(z), w(w){};

    M4_T<T> operator*(M4_T<T> m)
    {
        return M4_T<T>(m.x * (*this), m.y * (*this), m.z * (*this), m.w * (*this));
    }

    T* front()
    {
        return &x.x;
    }

    void translate(V3_T<T> v)
    {
        w = V4_T<T>(v, w.w);
    }
};

template <typename T>
void out(M4_T<T> m)
{
    out(m.x);
    out(m.y);
    out(m.z);
    out(m.w);
}

template <typename T>
M4_T<T> transpose(M4_T<T> m)
{
    return M4_T<T>(
        V4_T<T>(m.x.x, m.y.x, m.z.x, m.w.x),
        V4_T<T>(m.x.y, m.y.y, m.z.y, m.w.y),
        V4_T<T>(m.x.z, m.y.z, m.z.x, m.w.z),
        V4_T<T>(m.x.w, m.y.w, m.z.w, m.w.w));
}

template <typename T>
M4_T<T> translate(M4_T<T> m, V3_T<T> v)
{
    return M4_T<T>(
        m.x,
        m.y,
        m.z,
        V4_T<T>(v, 1) * m);
}

M4_T<F> lookAt(V3_T<F> position, V3_T<F> at, V3_T<F> up)
{
    V3_T<F> zaxis = normalize(at - position);
    V3_T<F> xaxis = normalize(cross(zaxis, up));
    V3_T<F> yaxis = cross(xaxis, zaxis);

    M4_T<F> view = M4_T<F>();
    view.x.x = xaxis.x;
    view.y.x = xaxis.y;
    view.z.x = xaxis.z;
    view.x.y = yaxis.x;
    view.y.y = yaxis.y;
    view.z.y = yaxis.z;
    view.x.z = -zaxis.x;
    view.y.z = -zaxis.y;
    view.z.z = -zaxis.z;
    view.w.x = -dot(xaxis, at);
    view.w.y = -dot(yaxis, at);
    view.w.z = dot(zaxis, at) - 1.0f;
    view.w.w = 1.0f;
    return view;
}

M4_T<F> perspective(F fovy, F aspect, F zNear, F zFar)
{
    // assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

    F tanHalfFovy = tan(fovy / 2.0f);

    M4_T<F> m = M4_T<F>();
    m.x.x = 1.0f / (aspect * tanHalfFovy);
    m.y.y = 1.0f / (tanHalfFovy);
    m.z.z = -(zFar + zNear) / (zFar - zNear);
    m.z.w = -1.0f;
    m.w.z = (-2.0f * zFar * zNear) / (zFar - zNear);
    return m;
}

// clang-format off
M4_T<F> scale(F n)
{
    return M4_T<F>(
        V4_T<F>(n, 0, 0, 0),
        V4_T<F>(0, n, 0, 0),
        V4_T<F>(0, 0, n, 0),
        V4_T<F>(0, 0, 0, 1)
    );
}

M4_T<F> translation(V3_T<F> v)
{
    return M4_T<F>(
        V4_T<F>(1, 0, 0, 0),
        V4_T<F>(0, 1, 0, 0),
        V4_T<F>(0, 0, 1, 0),
        V4_T<F>(      v, 1)
    );
}

M4_T<F> rotation(F theta, V3_T<F> v)
{
    v = normalize(v);

    return M4_T<F>(
        V4_T<F>(      cos(theta) + (1 - cos(theta)) * v.x * v.x, (1 - cos(theta)) * v.x * v.y + v.z * sin(theta), (1 - cos(theta)) * v.x * v.z - v.y * sin(theta), 0),
        V4_T<F>((1 - cos(theta)) * v.x * v.y - v.z * sin(theta),       cos(theta) + (1 - cos(theta)) * v.y * v.y, (1 - cos(theta)) * v.y * v.z + v.x * sin(theta), 0),
        V4_T<F>((1 - cos(theta)) * v.x * v.z + v.y * sin(theta), (1 - cos(theta)) * v.y * v.z - v.x * sin(theta),       cos(theta) + (1 - cos(theta)) * v.z * v.z, 0),
        V4_T<F>(                                              0,                                               0,                                               0, 1)
    );
}

M4_T<F> rotationX(F theta)
{
    return M4_T<F>(
        V4_T<F>(1,           0,          0, 0),
        V4_T<F>(0,  cos(theta), sin(theta), 0),
        V4_T<F>(0, -sin(theta), cos(theta), 0),
        V4_T<F>(0,           0,          0, 1)
    );
}

M4_T<F> rotationY(F theta)
{
    return M4_T<F>(
        V4_T<F>(cos(theta), 0, -sin(theta), 0),
        V4_T<F>(         0, 1,           0, 0),
        V4_T<F>(sin(theta), 0,  cos(theta), 0),
        V4_T<F>(         0, 0,           0, 1)
    );
}

M4_T<F> rotationZ(F theta)
{
    return M4_T<F>(
        V4_T<F>( cos(theta), sin(theta), 0, 0),
        V4_T<F>(-sin(theta), cos(theta), 0, 0),
        V4_T<F>(          0,          0, 1, 0),
        V4_T<F>(          0,          0, 0, 1)
    );
}
// clang-format on

typedef V2_T<F> V2;
typedef V3_T<F> V3;
typedef V4_T<F> V4;

typedef M2_T<F> M2;
typedef M3_T<F> M3;
typedef M4_T<F> M4;
