#ifndef LAYOUTRECONSTRUCTOR_H
#define LAYOUTRECONSTRUCTOR_H

#include "minidocx.hpp"
#include <vector>
#include <algorithm>
#include "json.hpp"
using json = nlohmann::json;

//文档重建
class LayoutReconstructor {
public:
    void operator()(const json& data, const std::string& path) {
        using namespace docx;

        Document doc;
        Section section = InitDocumentLayout(doc);

        for (const auto& obj : data["text_objects"]) {
            // 解析box坐标
            std::vector<std::vector<int>> box;
            for (const auto& pt : obj["box"]) {
                box.push_back({pt[0].get<int>(),  pt[1].get<int>()});
            }

            auto [x, y, w, h] = CalculatePosition(box);
            TextFrame tf = CreateTextFrame(doc, w, h);
            PositionTextFrame(tf, x, y);
            SetFrameStyle(tf);  // 修正边框设置

            // 直接使用TextFrame作为段落容器（关键修改点1）
            Paragraph para = tf;
            std::string text = obj["text"].get<std::string>();
            SetTextContent(para, text, h);
        }

        doc.Save(path);
    }

private:
    docx::Section InitDocumentLayout(docx::Document& doc) {
        docx::Section section = doc.FirstSection();
        section.SetPageSize(docx::Inch2Twip(docx::A4_W),
                            docx::Inch2Twip(docx::A4_H));
        section.SetPageMargin(0, 0, 0, 0);
        return section;
    }

    std::tuple<int, int, int, int> CalculatePosition(const std::vector<std::vector<int>>& box) {
        std::vector<int> xs, ys;
        for (const auto& pt : box) {
            xs.push_back(pt[0]);
            ys.push_back(pt[1]);
        }

        auto [x_min, x_max] = std::minmax_element(xs.begin(),  xs.end());
        auto [y_min, y_max] = std::minmax_element(ys.begin(),  ys.end());

        const int PPI = 72;
        return {
            *x_min * 1440 / PPI,
            *y_min * 1440 / PPI,
            (*x_max - *x_min) * 1440 / PPI,
            (*y_max - *y_min) * 1440 / PPI
        };
    }

    docx::TextFrame CreateTextFrame(docx::Document& doc, int w, int h) {
        docx::TextFrame tf = doc.AppendTextFrame(w, h);
        tf.SetTextWrapping(docx::TextFrame::Wrapping::None);
        return tf;
    }

    void PositionTextFrame(docx::TextFrame& tf, int x, int y) {
        tf.SetPositionX(x, docx::TextFrame::Anchor::Page);
        tf.SetPositionY(y, docx::TextFrame::Anchor::Page);
    }

    void SetFrameStyle(docx::TextFrame& tf) {
        // 关键修改点2：使用正确的边框设置方法
        tf.SetBorders(docx::Box::BorderStyle::None);
    }

    void SetTextContent(docx::Paragraph& para, const std::string& text, int frameHeight) {
        docx::Run run = para.AppendRun(text);

        // 根据文本框高度估算字体大小
        double fontSize = frameHeight * 0.6 / 20.0; // 转换为磅值
        run.SetFontSize(fontSize > 8 ? fontSize : 8);
        para.SetAlignment(docx::Paragraph::Alignment::Left);
    }
};
#endif // LAYOUTRECONSTRUCTOR_H
