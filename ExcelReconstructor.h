#ifndef EXCELRECONSTRUCTOR_H
#define EXCELRECONSTRUCTOR_H

#include "xlnt/xlnt.hpp"
#include <vector>
#include <algorithm>
#include "json.hpp"
using json = nlohmann::json;

#include <filesystem> // 添加头文件
namespace fs = std::filesystem;

class ExcelReconstructor {
public:
    void operator()(const json& data, const std::string& path) {
        try {
            // 1. 验证JSON结构
            if (!data.contains("cells")  || !data["cells"].is_array()) {
                throw std::runtime_error("Invalid JSON: missing 'cells' array");
            }

            // 2. 创建目标目录
            fs::path dir_path = fs::path(path).parent_path();
            if (!dir_path.empty()  && !fs::exists(dir_path)) {
                fs::create_directories(dir_path);
            }

            xlnt::workbook wb;
            auto ws = wb.active_sheet();
            // 3. 列宽计算准备
            std::unordered_map<int, size_t> max_col_lengths;  // <列索引, 最大字符数>
            const double CHAR_WIDTH_UNIT = 1.2;  // 字符宽度系数（根据实际显示效果调整）

            // 4. 首次遍历计算最大列宽
            for (const auto& cell : data["cells"]) {
                if (!cell.contains("col")  || !cell.contains("row"))  continue;

                int col = cell["col"].get<int>();
                std::string value = cell.contains("value")  ?
                                        cell["value"].get<std::string>() : "";

                // 计算有效显示长度（处理换行符等特殊字符）
                size_t display_length = value.length();
                if (!value.empty())  {
                    // 移除换行符影响
                    display_length = std::count_if(value.begin(),  value.end(),
                                                   [](char c) { return c != '\n' && c != '\r'; });
                }

                // 更新最大列宽记录
                if (display_length > max_col_lengths[col]) {
                    max_col_lengths[col] = display_length;
                }
            }

            // 5. 设置动态列宽（至少保持10字符宽度）
            for (const auto& [col_idx, max_len] : max_col_lengths) {
                double column_width = std::max(
                    static_cast<double>(max_len) * CHAR_WIDTH_UNIT,
                    10.0  // 最小宽度
                    );
                ws.column_properties(col_idx  + 1).width = column_width;
            }

            // 6. 获取有效边界
            int max_row = data.contains("grid_size")  ? data["grid_size"][0].get<int>() : 1000;
            int max_col = data.contains("grid_size")  ? data["grid_size"][1].get<int>() : 100;

            // 7. 安全填充数据
            for (const auto& cell : data["cells"]) {
                // 验证必要字段
                if (!cell.contains("col")  || !cell.contains("row"))  continue;

                int col = cell["col"].get<int>();
                int row = cell["row"].get<int>();

                // 边界检查
                if (col < 0 || col >= max_col || row < 0 || row >= max_row) continue;

                // 获取值（处理字段缺失）
                std::string value = cell.contains("value")  ? cell["value"].get<std::string>() : "";

                auto xl_cell = ws.cell(col+1,  row+1);

                // 数值处理优化
                if (!value.empty())  {
                    // 移除逗号后尝试转换数字
                    std::string clean_value = value;
                    clean_value.erase(std::remove(clean_value.begin(),  clean_value.end(),  ','), clean_value.end());

                    try {
                        size_t pos = 0;
                        double num_val = std::stod(clean_value, &pos);
                        if (pos == clean_value.size())  {
                            xl_cell.value(num_val);
                            continue;
                        }
                    } catch (...) {
                        // 转换失败保持字符串
                    }
                }
                xl_cell.value(value);
            }

            // 8. 安全保存
            wb.save(path);
            std::cout << "Excel重建成功: " << path << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "⚠️ 严重错误: " << e.what()  << std::endl;
            // 此处可添加日志记录
        }
    }
};

#endif // EXCELRECONSTRUCTOR_H
