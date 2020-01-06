#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>

//TODO:: define this in a user-based way
static const std::filesystem::path texture_path{"/home/sam/Dev/OpenGL/learning_projects/textures"};

//TODO:: optimize compile time
template <size_t storage_format, size_t input_format = storage_format, size_t texture_num = GL_TEXTURE0>
class Texture
{
public:
    explicit Texture(std::filesystem::path texPath)
    {
        glGenTextures(1, &texture);

        std::filesystem::path relPath = (texPath.is_absolute()) ? std::filesystem::relative(texPath) : texPath;

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(relPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glActiveTexture(texture_num);
            glBindTexture(GL_TEXTURE_2D, texture);

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, storage_format, width, height, 0, input_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR:: could not load image:" << texPath << "\n";
        }

        stbi_image_free(data);
    }

    Texture(const Texture &other) = delete;
    Texture &operator=(const Texture &other) = delete;
    Texture(Texture &&other)
    {
        texture = 0;
        swap(*this, other);
    }
    Texture &operator=(Texture &&other)
    {
        texture = 0;
        swap(*this, other);
    }

    ~Texture()
    {
        glDeleteTextures(1, &texture);
    }

    void bind()
    {
        glActiveTexture(texture_num);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    friend void swap(Texture &first, Texture &second)
    {
        std::swap(first.texture, second.texture);
    }

private:
    unsigned int texture{0};
};