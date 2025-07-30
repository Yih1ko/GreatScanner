#ifndef JIEBA_SINGLETON_H
#define JIEBA_SINGLETON_H

#include <mutex>
#include <memory>
#include "cppjieba/Jieba.hpp"

class JiebaSingleton {
public:
    // 删除拷贝构造函数和赋值运算符
    JiebaSingleton(const JiebaSingleton&) = delete;
    JiebaSingleton& operator=(const JiebaSingleton&) = delete;

    // 获取单例实例
    static cppjieba::Jieba& getInstance() {
        std::call_once(initFlag, []() {
            instance.reset(new  cppjieba::Jieba(
                DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH
                ));
        });
        return *instance;
    }

private:
    JiebaSingleton() = default;
    ~JiebaSingleton() = default;

    // 静态成员变量
    static constexpr const char* const DICT_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/jieba.dict.utf8";
    static constexpr const char* const HMM_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/hmm_model.utf8";
    static constexpr const char* const USER_DICT_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/user.dict.utf8";
    static constexpr const char* const IDF_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/idf.utf8";
    static constexpr const char* const STOP_WORD_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/stop_words.utf8";

    static inline std::unique_ptr<cppjieba::Jieba> instance;
    static inline std::once_flag initFlag;
};

// 初始化静态成员
//std::unique_ptr<cppjieba::Jieba> JiebaSingleton::instance;
//std::once_flag JiebaSingleton::initFlag;

#endif // JIEBA_SINGLETON_H
