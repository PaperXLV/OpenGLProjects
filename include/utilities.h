#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

template <typename T>
struct vec3
{
    using c_type = T;

    T x, y, z;

    constexpr vec3() : x{0}, y{0}, z{0}
    {
    }

    constexpr vec3(T x, T y, T z) : x{x}, y{y}, z{z}
    {
    }

    vec3(const vec3 &other) = default;
    vec3(vec3 &&other) = default;
    vec3 &operator=(const vec3 &other) = default;
    vec3 &operator=(vec3 &&other) = default;
    ~vec3() = default;

    friend std::ostream &operator<<(std::ostream &out, const vec3 &v)
    {
        out << "Vector:(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return out;
    }
};
