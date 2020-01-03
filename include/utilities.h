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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), &vertices[0], GLSetting);
    }

    void unbind()
    {
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
    VAO(const VAO &other) = delete;
    VAO &operator=(const VAO &other) = delete;
    VAO(VAO &&other)
    {
        swap(*this, other);
    }
    VAO &operator=(VAO &&other)
    {
        vao = 0;
        swap(*this, other);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &vao);
    }

    void bind()
    {
        glBindVertexArray(vao);
    }

    // replace with some generic, possibly taking a function object in VAO's constructor to call the attribs
    void setAttribs()
    {
        bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    friend void swap(VAO &first, VAO &second) noexcept
    {
        std::swap(first.vao, second.vao);
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

private:
    unsigned int shaderProgram{0};
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

class GLFWhandler
{
public:
    GLFWhandler()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        valid = true;
        // Parameters/globals for this at some point I think
        window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            valid = false;
        }
        else
        {
            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cerr << "Failed to initialize GLAD\n";
                glfwTerminate();
                valid = false;
            }
            else
            {
                glViewport(0, 0, 800, 600);
                glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            }
        }
    }

    friend void swap(GLFWhandler &first, GLFWhandler &second)
    {
        std::swap(first.window, second.window);
        std::swap(first.valid, second.valid);
    }

    GLFWhandler(const GLFWhandler &other) = delete;
    GLFWhandler &operator=(const GLFWhandler &other) = delete;
    GLFWhandler(GLFWhandler &&other)
    {
        valid = false;
        swap(*this, other);
    }
    GLFWhandler &operator=(GLFWhandler &&other)
    {
        valid = false;
        swap(*this, other);
    }

    void swapBuffers()
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool heartbeat()
    {
        return !glfwWindowShouldClose(window);
    }

    bool getValid()
    {
        return valid;
    }

    ~GLFWhandler()
    {
        glfwTerminate();
    }

private:
    GLFWwindow *window{nullptr};
    bool valid{false};
};