#include <iostream>

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
    T operator*(V2<T> v)
    {
        return x * v.x + y * v.y;
    }
    V2<T> operator*(M2<T> m)
    {
        return V2<T>(
            x * m.x.x + y * m.y.x,
            x * m.x.y + y * m.y.y);
    }

    void out()
    {
        std::cout << "[" << x << ", " << y << "]" << std::endl;
    }
};

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
    T operator*(V3<T> v)
    {
        return x * v.x + y * v.y + z * v.z;
    }
    V3<T> operator*(M3<T> m)
    {
        return V3<T>(
            x * m.x.x + y * m.y.x + z * m.z.x,
            x * m.x.y + y * m.y.y + z * m.z.y,
            x * m.x.z + y * m.y.z + z * m.z.z);
    }

    void out()
    {
        std::cout << "[" << x << ", " << y << ", " << z << "]" << std::endl;
    }
};

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
    T operator*(V4<T> v)
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }
    V4<T> operator*(M4<T> m)
    {
        return V4<T>(
            x * m.x.x + y * m.y.x + z * m.z.x + w * m.w.x,
            x * m.x.y + y * m.y.y + z * m.z.y + w * m.w.y,
            x * m.x.z + y * m.y.z + z * m.z.z + w * m.w.z,
            x * m.x.w + y * m.y.w + z * m.z.w + w * m.w.w);
    }

    void out()
    {
        std::cout << "[" << x << ", " << y << ", " << z << ", " << w << "]" << std::endl;
    }
};

// Matrix 2
template <typename T>
struct M2
{
    V2<T> x;
    V2<T> y;

    M2(){};
    M2(V2<T> x, V2<T> y) : x(x), y(y){};

    M2<T> operator*(M2<T> m)
    {
        return M2<T>(x * m, y * m);
    }

    void out()
    {
        x.out();
        y.out();
    }
};

// Matrix 3
template <typename T>
struct M3
{
    V3<T> x;
    V3<T> y;
    V3<T> z;

    M3(){};
    M3(V3<T> x, V3<T> y, V3<T> z) : x(x), y(y), z(z){};

    M3<T> operator*(M3<T> m)
    {
        return M3<T>(x * m, y * m, z * m);
    }

    void out()
    {
        x.out();
        y.out();
        z.out();
    }
};

// Matrix 4
template <typename T>
struct M4
{
    V4<T> x;
    V4<T> y;
    V4<T> z;
    V4<T> w;

    M4(){};
    M4(V4<T> x, V4<T> y, V4<T> z, V4<T> w) : x(x), y(y), z(z), w(w){};

    M4<T> operator*(M4<T> m)
    {
        return M4<T>(x * m, y * m, z * m, w * m);
    }

    void out()
    {
        x.out();
        y.out();
        z.out();
        w.out();
    }
};
