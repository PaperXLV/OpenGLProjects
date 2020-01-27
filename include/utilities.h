#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>
#include <string_view>

using namespace std::literals;

template <typename T>
struct vec3
{
    using c_type = T;
    static constexpr std::string_view name{"vec3"};

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

template <typename T>
struct mat4
{
    using c_type = T;
    static constexpr std::string_view name{"mat4"};

    glm::mat<4, 4, T> data;

    // Default operations?
    constexpr mat4(glm::mat<4, 4, T> m) : data{m}
    {
    }

    friend std::ostream &operator<<(std::ostream &out, const mat4 &m)
    {
        out << "Matrix: to be implimented\n";
        return out;
    }
};