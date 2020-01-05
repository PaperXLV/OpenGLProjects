#include "utility"
#include <type_traits>

template <typename T>
void setNull(T data)
{
}
/*
template <typename T, typename func>
class Uniform;

template <typename T, typename func>
void setDefaultUpdate(Uniform<T, func> &uni);
*/
//template <typename T, typename func = void (*)(T)>
template <typename T, typename func = void (*)(T)>
class Uniform
{
public:
    Uniform()
    {
        std::cout << "ILLEGAL\n";
    }
    Uniform(std::string name, const unsigned int shaderProgram, T data, func updateFunc = setNull) : name{name}, shaderProgram{shaderProgram}, data{data}, updateFunc{updateFunc}
    {
        std::cout << "INSTANTIATING UNIFORM WITH NAME: " << name << "\nPROGRAM: " << shaderProgram << "\n";
    }
    Uniform(const Uniform &other)
    {
        std::cout << "COPY CONSTRUCTOR \n";

        name = other.name;
        shaderProgram = other.shaderProgram;
        data = other.data;
        updateFunc = other.updateFunc;
    }
    Uniform(Uniform &&other)
    {
        std::cout << "MOVE CONSTRUCTOR \n";
        name = "";
        shaderProgram = 0;
        // not touching data. If I add dynamic I need to clear it
        updateFunc = setNull;
        swap(*this, other);
    }
    Uniform &operator=(const Uniform &other)
    {
        std::cout << "COPY ASSIGNMENT\n";
        Uniform cpy = other;
        swap(*this, cpy);
    }
    Uniform &operator=(Uniform &&other)
    {
        std::cout << "MOVE ASSIGNMENT\n";
        name = "";
        shaderProgram = 0;
        updateFunc = setNull;
        swap(*this, other);
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

    // Calls a function passed by template parameter on data.
    // Otherwise, I would have to find some way to constexpr if
    // my way through all the possible data types T can be.
    void update()
    {
        setDefaultUpdate(data);
        //updateFunc(data);
    }

    // Default funcs to use

    void setDefaultUpdate(T data)
    {
        int loc = glGetUniformLocation(shaderProgram, name.c_str());
        std::cout << "LOOKING FOR " << name << " IN PROGRAM: " << shaderProgram << "\n"
                  << "LOCATION: " << loc << "\n";

        if constexpr (std::is_class_v<T>)
        {
            if constexpr (std::is_floating_point_v<typename T::c_type>)
            {
                std::cout
                    << data;
                glUniform3f(loc, data.x, data.y, data.z);
            }
            // watch out for booleans, idk if they will bug or not
            else if constexpr (std::is_integral_v<typename T::c_type>)
            {
                glUniform3i(loc, data.x, data.y, data.z);
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

    //friend void setDefaultUpdate<T, func>(Uniform<T, func> &uni);

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
};
/*
template <typename T, typename func>
void setDefaultUpdate(Uniform<T, func> &uni)
{
    //std::cout << name << "\n";
    //int loc = glGetUniformLocation(shaderProgram, name.c_str());
    int loc = glGetUniformLocation(uni.shaderProgram, "color");
    std::cout << "LOOKING FOR "
              << " IN PROGRAM: " << uni.shaderProgram << "\n"
              << "LOCATION: " << loc << "\n";

    if constexpr (std::is_class_v<T>)
    {
        if constexpr (std::is_floating_point_v<typename T::c_type>)
        {
            std::cout << "FP VECTOR\n"
                      << uni.data;
            //glUniform3f(loc, uni.data.x, uni.data.y, uni.data.z);
            glUniform3f(loc, 1.0f, 0.0f, 0.0f);
        }
        // watch out for booleans, idk if they will bug or not
        else if constexpr (std::is_integral_v<typename T::c_type>)
        {
            std::cout << "INT VECTOR\n";
            glUniform3i(loc, uni.data.x, uni.data.y, uni.data.z);
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
            glUniform1f(loc, uni.data);
        }
        else if constexpr (std::is_integral_v<T>)
        {
            glUniform1i(loc, (int)uni.data);
        }
        else
        {
            std::cout << "unsupported type for setTrivial\n";
        }
    }
}
*/