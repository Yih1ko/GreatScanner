#ifndef SEARCHDEF_H
#define SEARCHDEF_H
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>
#include <iterator>

using RecordId = uint64_t;

    enum class Page{//搜索所属页面
        PAGE_TOOLS,//工具页面
        PAGE_HISTORY,//历史记录页面
        PAGE_BACKUP,//备份页面
        PAGE_SETTINGS,//设置页面
    };

    template<Page P>
    struct SearchMetaData{
        double match_score{0.0};//相关度
    };

    template<>//特化属于哪个页面的搜索元数据
    struct SearchMetaData<Page::PAGE_TOOLS>{
        // 添加默认构造函数
        SearchMetaData() = default;
        //参数：功能类型，功能
        SearchMetaData(const std::string& h, const std::string& func)
            :header(h), function(func)
        {}
        std::string header;
        std::string function;
    };

    template<>
    struct SearchMetaData<Page::PAGE_HISTORY>{
        SearchMetaData()
        {

        }
    private:
    };

    template<>
    struct SearchMetaData<Page::PAGE_BACKUP>{
        SearchMetaData()
        {

        }
    private:
    };

    template<>
    struct SearchMetaData<Page::PAGE_SETTINGS>{
        SearchMetaData()
        {

        }
    private:
    };

    using MetaDataVariant = std::variant<
        SearchMetaData<Page::PAGE_TOOLS>,
        SearchMetaData<Page::PAGE_HISTORY>,
        SearchMetaData<Page::PAGE_BACKUP>,
        SearchMetaData<Page::PAGE_SETTINGS>
        >;


#endif // SEARCHDEF_H

