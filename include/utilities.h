#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

template <typename T>
struct vec3
{
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
};

template <typename T, size_t GLSetting>
class VBO
{
public:
    VBO()
    {
        glGenBuffers(1, &buffer);
    }

    VBO(const std::vector<T> &vertData) : VBO()
    {
        vertices = vertData;
    }

    VBO(std::vector<T> &&vertData) : VBO()
    {
        vertices = vertData;
    };

    //Array implimentation is too difficult. Return to later

    VBO(const VBO &other) = delete;
    VBO &operator=(const VBO &other) = delete;
    VBO(VBO &&other)
    {
        swap(*this, other);
    }
    VBO &operator=(VBO &&other)
    {
        glDeleteBuffers(1, &buffer);
        buffer = 0;
        swap(*this, other);
    }

    ~VBO()
    {
        glDeleteBuffers(1, &buffer);
    }

    void bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GLSetting);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    GLuint buffer{0};
    std::vector<T> vertices{};
};

class VAO
{
public:
    VAO()
    {
        glGenVertexArrays(1, &vao);
    }

    void bind()
    {
        glBindVertexArray(vao);
    }

    // replace with some generic, possibly taking a function object in VAO's constructor to call the attribs
    void setAttribs()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }

private:
    unsigned int vao;
};

template <size_t GL_Shader_Type>
class Shader
{
public:
    Shader(const char *source)
    {
        shader = glCreateShader(GL_Shader_Type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << infoLog << "\n";
        }
    }

    Shader(const Shader &other) = delete;
    Shader &operator=(const Shader &other) = delete;
    Shader(Shader &&other) : Shader()
    {
        swap(*this, other);
    }
    Shader &operator=(Shader &&other)
    {
        shader = 0;
        swap(*this, other);
    }
    ~Shader()
    {
        glDeleteShader(shader);
    }

    friend void swap(Shader &first, Shader &second) noexcept
    {
        std::swap(first.shader, second.shader);
    }

    unsigned int getShader() const { return shader; }

private:
    unsigned int shader{0};
};

class ShaderProgram
{
public:
    ShaderProgram(const char *vertexSource, const char *fragSource, const char *geomSource = nullptr)
    {
        Shader<GL_VERTEX_SHADER> vertex{vertexSource};
        Shader<GL_FRAGMENT_SHADER> frag{fragSource};

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex.getShader());
        glAttachShader(shaderProgram, frag.getShader());

        if (geomSource != nullptr)
        {
            Shader<GL_GEOMETRY_SHADER> geom{geomSource};
            glAttachShader(shaderProgram, geom.getShader());
        }

        glLinkProgram(shaderProgram);
    }
    //TODO:: Define copy move and destructors

    void use()
    {
        glUseProgram(shaderProgram);
    }

private:
    unsigned int shaderProgram{0};
};