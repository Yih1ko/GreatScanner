#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "SearchDef.h"
#include "cppjieba/Jieba.hpp"
#include "JiebaSingleton.h"

// Trie节点结构
struct TrieNode {
    std::map<char, std::shared_ptr<TrieNode>> children;
    std::set<RecordId> ids; // 存储经过该节点的所有ID
};

template <Page P>
class SearchEngine {
    using MetaDataMap = std::unordered_map<RecordId, SearchMetaData<P>>;
    // static constexpr const char* const DICT_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/jieba.dict.utf8";
    // static constexpr const char* const HMM_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/hmm_model.utf8";
    // static constexpr const char* const USER_DICT_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/user.dict.utf8";
    // static constexpr const char* const IDF_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/idf.utf8";
    // static constexpr const char* const STOP_WORD_PATH = "D:/Qt_Projects/GreatScanner/search/cppjieba/dict/stop_words.utf8";
public:
    SearchEngine() : m_trieRoot(std::make_shared<TrieNode>())/*,
        m_jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)*/ {}

    // 插入记录（线程安全）
    void insertRecord(const std::string& title, RecordId id, const SearchMetaData<P>& metaData) {
        std::lock_guard<std::mutex> lock(m_mutex);

        // 1. 存储标题和元数据
        m_titles[id] = title;
        m_metaDataMap[id] = metaData;

        // 2. 插入Trie树
        insertIntoTrie(title, id);

        // 3. 插入倒排索引
        insertIntoInvertedIndex(title, id);
    }

    // 删除记录（线程安全）
    void deleteRecord(RecordId id) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (auto it = m_titles.find(id);  it != m_titles.end())  {
            const std::string title = it->second;

            // 1. 从Trie树删除
            deleteFromTrie(title, id);

            // 2. 从倒排索引删除
            deleteFromInvertedIndex(title, id);

            // 3. 清除元数据
            m_metaDataMap.erase(id);
            m_titles.erase(id);
        }
    }

    // 前缀搜索（自动补全）
    std::set<RecordId> getPrefixIds(const std::string& prefix) const {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto node = m_trieRoot;
        for (char c : prefix) {
            if (auto it = node->children.find(c);  it != node->children.end())  {
                node = it->second;
            } else {
                return {};
            }
        }
        return node->ids; // 返回所有匹配ID
    }

    // 关键词搜索（按匹配度排序）
    std::vector<RecordId> searchKeywords(const std::string& keywords) const {
        std::lock_guard<std::mutex> lock(m_mutex);

        // 1. 分词并归一化
        const std::vector<std::string> kwList = splitAndNormalize(keywords);
        if (kwList.empty())  return {};

        // 2. 获取关键词对应的ID集合
        std::vector<std::set<RecordId>> idSets;
        for (const auto& word : kwList) {
            if (auto it = m_invertedIndex.find(word);  it != m_invertedIndex.end())  {
                idSets.push_back(it->second);
            } else {
                return {}; // 任一关键词无匹配则返回空
            }
        }

        // 3. 计算交集
        std::set<RecordId> resultSet = idSets[0];
        for (size_t i = 1; i < idSets.size();  ++i) {
            std::set<RecordId> temp;
            std::set_intersection(
                resultSet.begin(),  resultSet.end(),
                idSets[i].begin(), idSets[i].end(),
                std::inserter(temp, temp.begin())
                );
            resultSet = std::move(temp);
        }

        // 4. 计算匹配度并排序
        std::vector<std::pair<RecordId, double>> scoredResults;
        for (RecordId id : resultSet) {
            scoredResults.emplace_back(id,  computeMatchScore(m_titles.at(id),  kwList));
        }

        std::sort(scoredResults.begin(),  scoredResults.end(),
                  [](const auto& a, const auto& b) { return a.second  > b.second;  });

        // 5. 提取排序后的ID
        std::vector<RecordId> sortedIds;
        for (const auto& [id, score] : scoredResults) {
            sortedIds.push_back(id);
        }
        return sortedIds;
    }

    // 获取元数据
    const SearchMetaData<P>& getMetaData(RecordId id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_metaDataMap.at(id);
    }

private:
    // 核心数据结构
    std::shared_ptr<TrieNode> m_trieRoot;
    // 新增jieba分词器成员(静态)
    // cppjieba::Jieba m_jieba;
    // static inline cppjieba::Jieba m_jieba {
    //     DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH
    // };
    std::unordered_map<std::string, std::set<RecordId>> m_invertedIndex;
    MetaDataMap m_metaDataMap;
    std::unordered_map<RecordId, std::string> m_titles; // ID到标题的映射
    mutable std::mutex m_mutex;

    // 插入Trie树
    void insertIntoTrie(const std::string& title, RecordId id) {
        auto node = m_trieRoot;
        for (char c : title) {
            if (!node->children.count(c))  {
                node->children[c] = std::make_shared<TrieNode>();
            }
            node = node->children[c];
            node->ids.insert(id);  // 每个节点存储ID
        }
    }

    // 从Trie树删除
    void deleteFromTrie(const std::string& title, RecordId id) {
        std::vector<std::shared_ptr<TrieNode>> path; // 记录访问路径
        auto node = m_trieRoot;

        // 遍历并记录路径
        for (char c : title) {
            if (auto it = node->children.find(c);  it != node->children.end())  {
                path.push_back(node);
                node = it->second;
                node->ids.erase(id);
            } else break;
        }

        // 反向清理空节点
        for (auto it = path.rbegin();  it != path.rend();  ++it) {
            auto parent = *it;
            for (auto& [ch, child] : parent->children) {
                if (child->ids.empty()  && child->children.empty())  {
                    parent->children.erase(ch);  // 删除叶子空节点
                }
            }
        }
    }

    // 插入倒排索引
    void insertIntoInvertedIndex(const std::string& title, RecordId id) {
        for (const std::string& word : splitAndNormalize(title)) {
            m_invertedIndex[word].insert(id);
        }
    }

    // 从倒排索引删除
    void deleteFromInvertedIndex(const std::string& title, RecordId id) {
        for (const std::string& word : splitAndNormalize(title)) {
            if (auto it = m_invertedIndex.find(word);  it != m_invertedIndex.end())  {
                it->second.erase(id);
                if (it->second.empty())  {
                    m_invertedIndex.erase(it);
                }
            }
        }
    }

    // 分词和归一化（小写转换）
    std::vector<std::string> splitAndNormalize(const std::string& text) const {
        std::vector<std::string> words;

        // 通过单例管理器获取jieba实例
        auto& jieba = JiebaSingleton::getInstance();
        jieba.CutForSearch(text, words);

        // 归一化处理
        std::vector<std::string> normalizedWords;
        for (auto& word : words) {
            std::transform(word.begin(),  word.end(),  word.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            normalizedWords.push_back(std::move(word));
        }

        return normalizedWords;
    }

    // 匹配度计算算法
    double computeMatchScore(const std::string& title,
                             const std::vector<std::string>& keywords) const {
        double score = 0.0;
        // 使用jieba对标题分词
        std::vector<std::string> titleWords = splitAndNormalize(title);

        // 1. 词频统计（基于分词结果）
        for (const auto& kw : keywords) {
            for (const auto& titleWord : titleWords) {
                if (titleWord == kw) {
                    score += 2.0;  // 完整词匹配权重更高
                }
                // 子串匹配（处理长尾词）
                else if (titleWord.find(kw)  != std::string::npos) {
                    score += 0.5;
                }
            }
        }

        // 2. 位置权重优化（首词匹配权重提升）
        if (!titleWords.empty()  && titleWords.front()  == keywords.front())  {
            score += 3.0;
        }
        // 标题包含完整关键词序列（额外奖励）
        else if (std::search(titleWords.begin(),  titleWords.end(),
                             keywords.begin(),  keywords.end())  != titleWords.end())  {
            score += 2.5;
        }

        return score;
    }
};

// 安全访问接口(暂时没用上)
template <Page P>
class SafeSearchResult {
public:
    SafeSearchResult(SearchEngine<P>& engine, std::string query)
        : m_engine(engine), m_query(std::move(query)) {}

    std::vector<RecordId> get() {
        std::lock_guard<std::mutex> lock(m_engine.m_mutex);
        return m_engine.searchKeywords(m_query);  // 在锁保护下执行
    }
private:
    SearchEngine<P>& m_engine;
    std::string m_query;
};
#endif // SEARCHENGINE_H
