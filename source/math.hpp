
// Vector 2
template <typename T>
struct V2
{
    T x;
    T y;

    V2(){};

    V2(T x, T y) : x(x), y(y){};
};

template <typename T>
T dot(V2<T> a, V2<T> b)
{
    return a.x * b.x + a.y * b.y;
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
};

template <typename T>
T dot(V3<T> a, V3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T cross(V3<T> a, V3<T> b)
{
    return a.y * b.z - a.z * b.y - a.x * b.z + a.z * b.x + a.x * b.y - a.y * b.x;
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
};

template <typename T>
T dot(V4<T> a, V4<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Matrix 2
// template <typename T>
// struct M2 {
//     V2<T> x;
//     V2<T> y;

// }
