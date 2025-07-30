#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <memory>
#include <mutex>
#include <iostream>

template <typename T>
class _Singleton {
protected:
    _Singleton() = default;
    _Singleton(const _Singleton<T>&) = delete;
    _Singleton& operator=(const _Singleton<T>& st) = delete;

    static std::shared_ptr<T> _instance;
public:
    // static std::shared_ptr<T> GetInstance() {
    //     static std::once_flag s_flag;
    //     std::call_once(s_flag, [&]() {
    //         _instance = std::shared_ptr<T>(new T);
    //     });
    //     return _instance;
    // }
    //
    // template<typename... Args>
    // static std::shared_ptr<T> GetInstance(Args&&... args) {
    //     static std::once_flag s_flag;
    //     std::call_once(s_flag, [&]() {
    //         _instance = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
    //     });
    //     return _instance;
    // }
    template<typename... Args>
    static std::shared_ptr<T> GetInstance(Args&&... args) {
        static std::once_flag s_flag;
        std::call_once(s_flag, [args...] {
            if constexpr (sizeof...(Args) == 0) {//无参构造
                _instance = std::shared_ptr<T>(new T);
            } else {//无有参构造
                _instance = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
            }
        });
        return _instance;
    }
    static void DestroyInstance(){
        _instance.reset();
    }
    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }
    ~_Singleton() {
        std::cout << "this is singleton <" << typeid(T).name() <<"> destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> _Singleton<T>::_instance = nullptr;

#endif // _SINGLETON_H
