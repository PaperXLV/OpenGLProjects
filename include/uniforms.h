#include "utility"
#include <type_traits>

template <typename T>
void setNull(T data)
{
}

template <typename T, typename func = void (*)(T)>
class Uniform
{
public:
    Uniform(std::string name, const unsigned int shaderProgram, T data) : name{name}, shaderProgram{shaderProgram}, data{data}, updateFunc{setNull}, alternateFuncFlag{false}
    {
        update();
    }
    Uniform(std::string name, const unsigned int shaderProgram, T data, func updateFunc) : name{name}, shaderProgram{shaderProgram}, data{data}, updateFunc{updateFunc}, alternateFuncFlag{true}
    {
        update();
    }
    Uniform(const Uniform &other) = delete;
    Uniform(Uniform &&other)
    {
        name = "";
        shaderProgram = 0;
        // not touching data. If I add dynamic I need to clear it
        updateFunc = setNull;
        swap(*this, other);
        update();
    }
    Uniform &operator=(const Uniform &other) = delete;
    Uniform &operator=(Uniform &&other)
    {
        name = "";
        shaderProgram = 0;
        updateFunc = setNull;
        swap(*this, other);
        update();
    }

    void setData(const T &newData)
    {
        data = newData;
    }

    void setData(T &&newData)
    {
        data = newData;
    }

    void setName(const std::string &newName)
    {
        name = newName;
    }

    // Check alternateFuncFlag to see if user supplied a different function for update
    // Doing this so the user does not have to create sub-uniform classes while getting
    // custom functionality out of them.
    // TODO:: make update call on construction?
    void update()
    {
        if (!alternateFuncFlag)
        {
            defaultUpdate(data);
        }
        else
        {
            updateFunc(data);
        }
    }

    // Default funcs to use
    // Note:: using defaults requires T has typename::c_type and name if it's a class
    void defaultUpdate(T data)
    {
        int loc = glGetUniformLocation(shaderProgram, name.c_str());

        if constexpr (std::is_class_v<T>)
        {
            if constexpr (std::is_floating_point_v<typename T::c_type>)
            {
                if constexpr (data.name == "vec3"sv)
                {
                    glUniform3f(loc, data.x, data.y, data.z);
                }
                else if constexpr (data.name == "mat4"sv)
                {
                    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data.data));
                }
                else
                {
                    std::cout << "unsupported type for nonTrivial default\n";
                }
            }
            // watch out for booleans, idk if they will bug or not
            else if constexpr (std::is_integral_v<typename T::c_type>)
            {
                if constexpr (data.name == "vec3"sv)
                {
                    glUniform3i(loc, data.x, data.y, data.z);
                }
                else if constexpr (data.name == "mat4"sv)
                {
                    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data.data));
                }
                else
                {
                    std::cout << "unsupported type for nonTrivial default\n";
                }
            }
            else
            {
                std::cout << "unsupported type for nonTrivial default\n";
            }
        }
        else
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                glUniform1f(loc, data);
            }
            else if constexpr (std::is_integral_v<T>)
            {
                glUniform1i(loc, (int)data);
            }
            else
            {
                std::cout << "unsupported type for setTrivial\n";
            }
        }
    }

    friend void swap(Uniform &first, Uniform &second)
    {
        using std::swap;
        swap(first.name, second.name);
        swap(first.shaderProgram, second.shaderProgram);
        swap(first.data, second.data);
        swap(first.updateFunc, second.updateFunc);
    }

private:
    std::string name{};
    unsigned int shaderProgram{};
    T data{};
    func updateFunc{setNull};
    bool alternateFuncFlag{false};
};
