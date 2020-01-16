#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "math.hpp"

V3 toM(glm::vec3 v)
{
    return V3(v.x, v.y, v.z);
}

void out(F32 n)
{
    std::cout << "OUT: " << n << std::endl;
}
void out(const char** s)
{
    std::cout << "OUT: " << s << std::endl;
}
void out(glm::vec3 v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}
void out(glm::vec4 v)
{
    std::cout << "OUT: "
              << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]" << std::endl;
}
void out(glm::mat3 m)
{
    out(m[0]);
    out(m[1]);
    out(m[2]);
}
void out(glm::mat4 m)
{
    out(m[0]);
    out(m[1]);
    out(m[2]);
    out(m[3]);
}

int main()
{
    // tests
    glm::vec3 glm_v3_a = {1, 2, 3};
    V3        v3_a = {1, 2, 3};

    glm::vec3 glm_v3_b = {5, 4, 3};
    V3        v3_b = {5, 4, 3};

    std::cout << "OUT: vec * vec" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm_v3_a * glm_v3_b);
    std::cout << "OUT: m" << std::endl;
    out(v3_a * v3_b);

    glm::vec4 glm_v4_a = {1, 2, 3, 4};
    V4        v4_a = {1, 2, 3, 4};

    glm::vec4 glm_v4_b = {5, 4, 3, 2};
    V4        v4_b = {5, 4, 3, 2};

    std::cout << "OUT: vec * vec" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm_v4_a * glm_v4_b);
    std::cout << "OUT: m" << std::endl;
    out(v4_a * v4_b);

    glm::mat3 glm_m3_a = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    M3 m3_a = {
        V3(1, 2, 3),
        V3(4, 5, 6),
        V3(7, 8, 9)};

    glm::mat3 glm_m3_b = {
        {9, 2, 3},
        {2, 3, 4},
        {3, 4, 5}};
    M3 m3_b = {
        V3(9, 2, 3),
        V3(2, 3, 4),
        V3(3, 4, 5)};
    glm::mat4 glm_m4_a = {
        {9, 2, 3, 6},
        {2, 3, 4, 6},
        {3, 4, 5, 6},
        {3, 4, 5, 6}};
    M4 m4_a = {
        V4(9, 2, 3, 6),
        V4(2, 3, 4, 6),
        V4(3, 4, 5, 6),
        V4(3, 4, 5, 6)};

    std::cout << "OUT: vec * mat" << std::endl;
    out(glm_v3_a * glm_m3_b);
    std::cout << "OUT: m" << std::endl;
    out(v3_a * m3_b);

    std::cout << "OUT: mat * mat" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm_m3_a * glm_m3_b);
    std::cout << "OUT: m" << std::endl;
    out(m3_a * m3_b);

    std::cout << "OUT: lookAt" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm::lookAt(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(3.0f, 2.0f, 1.0f), glm::vec3(0.5f, 1.0f, 0.2f)));
    std::cout << "OUT: m" << std::endl;
    out(lookAt(V3_T<F32>(1, 2, 3), V3_T<F32>(3, 2, 1), V3_T<F32>(0.5, 1.0, 0.2)));

    std::cout << "OUT: translate" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm::translate(glm_m4_a, glm_v3_a));
    std::cout << "OUT: m" << std::endl;
    out(translate(m4_a, v3_a));

    std::cout << "OUT: rotate" << std::endl;
    std::cout << "OUT: glm" << std::endl;
    out(glm::rotate(glm_m4_a, glm::radians(50.0f), glm_v3_a));
    std::cout << "OUT: m" << std::endl;
    out(m4_a * rotation(radians(50.0f), v3_a));
    return 1;
}