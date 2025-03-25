#pragma once

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
            s_instance = Scope<T>(new T{});
        }
        return *s_instance;
    }

    Singleton(const Singleton&)            = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton()  = default;
    ~Singleton() = default;

private:
    static Scope<T> s_instance;
};

template<typename T>
Scope<T> Singleton<T>::s_instance = nullptr;

}   // namespace crab
