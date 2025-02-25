#pragma once
#include <memory>

namespace crab
{

template<typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<T>(new T{});
        }
        return *s_instance;
    }

    Singleton(const Singleton&)            = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton()  = default;
    ~Singleton() = default;

private:
    static std::unique_ptr<T> s_instance;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::s_instance = nullptr;

}   // namespace crab
