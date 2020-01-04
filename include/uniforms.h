#include <string>
#include <type_traits>

template <typename T, typename func>
class Uniform
{
public:
    Uniform() = default;
    Uniform(std::string name, func updateFunc, T data = 0) : name{name}, updateFunc{updateFunc}, data{data}
    {
    }
    Uniform(const Uniform &other) = default;
    Uniform(Uniform &&other) = default;
    Uniform &operator=(const Uniform &other) = default;
    Uniform &operator=(Uniform &&other) = default;

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
        updateFunc(data);
    }

private:
    std::string name{};
    func updateFunc{};
    T data{};
};
