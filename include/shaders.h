#include <string>
#include "utility"
#include <type_traits>

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

//TODO:: have ShaderProgram manage it's own uniforms
class ShaderProgram
{
public:
    ShaderProgram(const char *vertexSource, const char *fragSource, const char *geomSource = nullptr)
    {
        Shader<GL_VERTEX_SHADER> vertex{vertexSource};
        Shader<GL_FRAGMENT_SHADER> frag{fragSource};

        shaderProgram = glCreateProgram();
        std::cout << "MADE PROGRAM: " << shaderProgram << "\n";
        glAttachShader(shaderProgram, vertex.getShader());
        glAttachShader(shaderProgram, frag.getShader());

        if (geomSource != nullptr)
        {
            Shader<GL_GEOMETRY_SHADER> geom{geomSource};
            glAttachShader(shaderProgram, geom.getShader());
        }

        glLinkProgram(shaderProgram);

        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << "\n";
        }
    }
    //TODO:: Define copy move and destructors

    void use()
    {
        glUseProgram(shaderProgram);
    }

    int getUniformLocation(const GLchar *name) const
    {
        return glGetUniformLocation(shaderProgram, name);
    }

    unsigned int getProgramNumber() const
    {
        std::cout << "returning program: " << shaderProgram << "\n";
        return shaderProgram;
    }

private:
    unsigned int shaderProgram{0};
};