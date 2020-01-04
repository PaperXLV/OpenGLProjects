#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <cmath>

#include "utilities.h"
#include "graphics.h"
#include "shaders.h"
#include "uniforms.h"

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

uniform vec4 green;

void main()
{
    FragColor = green;
})";

constexpr std::array<vec3<float>, 4> vertices{vec3{0.5f, 0.5f, 0.0f},
                                              vec3{0.5f, -0.5f, 0.0f},
                                              vec3{-0.5f, -0.5f, 0.0f},
                                              vec3{-0.5f, 0.5f, 0.0f}};

constexpr std::array<vec3<int>, 1> indices{
    vec3{0, 1, 3}};

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

    EBO<vec3<int>, GL_STATIC_DRAW> ebo{std::vector<vec3<int>>{indices.begin(), indices.end()}};
    VBO<vec3<float>, GL_STATIC_DRAW> vbo{std::vector<vec3<float>>{vertices.begin(), vertices.end()}};
    vbo.bind();
    ebo.bind();

    vao.setAttribs();
    vao.unbind();

    // Compiling shaders
    ShaderProgram sp{vertexShaderSource, fragmentShaderSource};

    sp.use();

    // Uniform
    int loc = sp.getUniformLocation("green");
    auto update = [=](const float data) { glUniform4f(loc, 0.0f, data, 0.0f, 1.0f); };
    Uniform<float, decltype(update)> uni{"green", update};

    // Main loop
    while (glfwhandle.heartbeat())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();

        float timeValue = glfwGetTime();
        float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
        uni.setData(greenValue);
        uni.update();

        vao.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        vao.unbind();

        glfwhandle.swapBuffers();
    }

    return 0;
}