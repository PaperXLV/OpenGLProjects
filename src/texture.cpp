#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <cmath>

#include "utilities.h"
#include "graphics.h"
#include "shaders.h"
#include "uniforms.h"
#include "textures.h"

static const char *vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
})glsl";

static const char *fragmentShaderSource = R"glsl(
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord * vec2(2.0, 2.0)), 0.2);
})glsl";

constexpr std::array<float, 32> vertexData{
    // positions          // colors           // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
};

constexpr std::array<vec3<int>, 4> indices{
    vec3{0, 1, 2}, vec3{0, 2, 3}};

int main()
{
    //GLFW
    GLFWhandler glfwhandle{};
    if (!glfwhandle.getValid())
    {
        std::cout << "GLFW initialization failed\n";
    }

    // VBO and VAO
    // TODO:: need a better way to determine stride and offset. Possibly some sort of n-vector for vertexData, then we can just use vec.size()
    auto attribFunc = []() {
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // tex attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    };

    VAO vao{attribFunc};
    vao.bind();

    VBO<float, GL_STATIC_DRAW> vbo{std::vector<float>{vertexData.begin(), vertexData.end()}};
    vbo.bind();

    EBO<vec3<int>, GL_STATIC_DRAW> ebo{std::vector<vec3<int>>{indices.begin(), indices.end()}};
    ebo.bind();

    vao.setAttribs();
    vao.unbind();

    // Compiling shaders
    ShaderProgram sp{vertexShaderSource, fragmentShaderSource};
    sp.use();

    Texture<GL_RGB> tex{texture_path / "container.jpg"};
    Texture<GL_RGB, GL_RGBA, GL_TEXTURE1> tex2{texture_path / "awesomeface.png"};

    Uniform<int> texHandler{"texture1", sp.getProgramNumber(), 0};
    Uniform<int> tex2Handler{"texture2", sp.getProgramNumber(), 1};

    // Main loop
    while (glfwhandle.heartbeat())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
        vao.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.unbind();

        glfwhandle.swapBuffers();
    }

    return 0;
}