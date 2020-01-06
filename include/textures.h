#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>

//TODO:: define this in a user-based way
static const std::filesystem::path texture_path{"/home/sam/Dev/OpenGL/learning_projects/textures"};

//TODO:: optimize compile time
template <size_t storage_format, size_t input_format = storage_format>
class Texture
{
public:
    Texture(std::filesystem::path texPath)
    {
        glGenTextures(1, &texture);

        std::filesystem::path relPath = (texPath.is_absolute()) ? std::filesystem::relative(texPath) : texPath;
        int width, height, nrChannels;
        unsigned char *data = stbi_load(relPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, storage_format, width, height, 0, input_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR:: could not load image:" << texPath << "\n";
        }

        stbi_image_free(data);
    }

    ~Texture()
    {
        glDeleteTextures(1, &texture);
    }

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

private:
    unsigned int texture{0};
};