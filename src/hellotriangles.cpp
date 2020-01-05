#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

#include "utilities.h"
#include "graphics.h"
#include "shaders.h"

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

static const char *fragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
})";

constexpr std::array<vec3<float>, 4> vertices{vec3{0.5f, 0.5f, 0.0f},
                                              vec3{0.5f, -0.5f, 0.0f},
                                              vec3{-0.5f, -0.5f, 0.0f},
                                              vec3{-0.5f, 0.5f, 0.0f}};

constexpr std::array<vec3<int>, 1> indices1{
    vec3{0, 1, 3}};

constexpr std::array<vec3<int>, 1> indices2{
    vec3{1, 2, 3}};

int main()
{
    //GLFW
    GLFWhandler glfwhandle{};
    if (!glfwhandle.getValid())
    {
        std::cout << "GLFW initialization failed\n";
    }

    // VBO and VAO
    auto vaoAttrib = []() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    };

    VAO vao{vaoAttrib};
    vao.bind();

    EBO<vec3<int>, GL_STATIC_DRAW> ebo{std::vector<vec3<int>>{indices1.begin(), indices1.end()}};
    VBO<vec3<float>, GL_STATIC_DRAW> vbo{std::vector<vec3<float>>{vertices.begin(), vertices.end()}};
    vbo.bind();
    ebo.bind();

    vao.setAttribs();
    vao.unbind();

    VAO vao2{vaoAttrib};
    vao2.bind();
    EBO<vec3<int>, GL_STATIC_DRAW> ebo2{std::vector<vec3<int>>{indices2.begin(), indices2.end()}};
    vbo.bind();
    ebo2.bind();

    vao2.setAttribs();
    vao2.unbind();

    // Compiling shaders
    ShaderProgram sp{vertexShaderSource, fragmentShaderSource};
    ShaderProgram sp2{vertexShaderSource, fragmentShaderSource2};

    sp.use();

    // Main loop
    while (glfwhandle.heartbeat())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
        vao.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        vao.unbind();
        sp2.use();
        vao2.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        vao2.unbind();

        glfwhandle.swapBuffers();
    }

    return 0;
}