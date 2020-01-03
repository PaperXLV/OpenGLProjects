#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "utilities.h"

static const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

static const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

constexpr std::array<vec3<float>, 3> vertices{vec3{-0.5f, -0.5f, 0.0f},
                                              vec3{0.5f, -0.5f, 0.0f},
                                              vec3{0.0f, 0.5f, 0.0f}};

int main()
{
    //GLFW
    GLFWhandler glfwhandle{};
    if (!glfwhandle.getValid())
    {
        std::cout << "GLFW initialization failed\n";
    }

    // VBO and VAO
    VAO vao{};
    vao.bind();
    VBO<vec3<float>, GL_STATIC_DRAW> vbo{std::vector<vec3<float>>{vertices.begin(), vertices.end()}};
    vbo.bind();
    vao.setAttribs();
    vbo.unbind();

    // Compiling shaders
    ShaderProgram sp{vertexShaderSource, fragmentShaderSource};

    // instructions for interpreting the vertex data
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    //glEnableVertexAttribArray(0);

    sp.use();

    // Main loop
    while (glfwhandle.heartbeat())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwhandle.swapBuffers();
    }

    return 0;
}