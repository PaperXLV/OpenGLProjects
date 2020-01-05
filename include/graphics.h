template <typename T, size_t GLSetting>
class VBO
{
public:
    explicit VBO()
    {
        glGenBuffers(1, &buffer);
    }

    VBO(const std::vector<T> &vertData) : VBO()
    {
        vertices = vertData;
        if (vertices.size() == 0)
        {
            std::cout << "WARNING::Empty data in VBO\n";
        }
    }

    VBO(std::vector<T> &&vertData) : VBO()
    {
        vertices = vertData;
        if (vertices.size() == 0)
        {
            std::cout << "WARNING::Empty data in VBO\n";
        }
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
        return *this;
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

// Needs work on the setAttrib function.
template <typename func>
class VAO
{
public:
    explicit VAO()
    {
        glGenVertexArrays(1, &vao);
    }
    explicit VAO(func attribFunc) : attribFunc{attribFunc}
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
        return *this;
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &vao);
    }

    void bind()
    {
        glBindVertexArray(vao);
    }

    void unbind()
    {
        glBindVertexArray(0);
    }

    void setAttribs()
    {
        bind();
        attribFunc();
    }

    friend void swap(VAO &first, VAO &second) noexcept
    {
        std::swap(first.vao, second.vao);
    }

private:
    unsigned int vao{0};
    func attribFunc{};
};

template <typename T, size_t GLSetting>
class EBO
{
public:
    explicit EBO()
    {
        glGenBuffers(1, &ebo);
    }
    EBO(const std::vector<T> &edata) : EBO()
    {
        data = edata;
        if (data.size() == 0)
        {
            std::cout << "WARNING::Empty data in EBO\n";
        }
    }
    EBO(std::vector<T> &&edata) : EBO()
    {
        data = edata;
        if (data.size() == 0)
        {
            std::cout << "WARNING::Empty data in EBO\n";
        }
    }
    EBO(const EBO &other) = delete;
    EBO &operator=(const EBO &other) = delete;
    EBO(EBO &&other)
    {
        ebo = 0;
        data.clear();
        swap(*this, other);
    }
    EBO &operator=(EBO &&other)
    {
        ebo = 0;
        data.clear();
        swap(*this, other);
    }
    ~EBO()
    {
        glDeleteBuffers(1, &ebo);
    }

    void bind()
    {

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GLSetting);
    }
    void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

private:
    unsigned int ebo{0};
    std::vector<T> data{};
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
        return *this;
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