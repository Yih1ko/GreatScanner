#include "cvutils.h"


cv::Mat CvUtils::tableStructure::eraseRegions(cv::Mat &image, const json &data)
{
    // 解析每个文本区域
    for (const auto& obj : data["text_objects"]) {
        std::vector<cv::Point> polygon;

        // 构建多边形顶点坐标
        for (const auto& pt : obj["box"]) {
            int x = pt[0].get<int>();
            int y = pt[1].get<int>();
            polygon.emplace_back(x, y);
        }

        // 创建填充掩模
        cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
        std::vector<std::vector<cv::Point>> contours = {polygon};
        fillPoly(mask, contours, cv::Scalar(255));

        // 高级擦除策略（三选一）
        // 1. 白色填充
        //image.setTo(cv::Scalar(255, 255, 255), mask);

        // 2. 黑色填充
        // image.setTo(Scalar(0, 0, 0), mask);

        // 3. 智能修复
        cv::Mat inpainted;
        inpaint(image, mask, inpainted, 3, cv::INPAINT_TELEA);
        inpainted.copyTo(image);
    }
    return image;
}


// 表格结构识别主函数实现
json CvUtils::tableStructure::exportToJson(const std::string& imgPath, const json& textInfo) {
    // 图像读取与预处理
    cv::Mat img = cv::imread(imgPath);  // 读取原始图像
    eraseRegions(img, textInfo);        // 擦除文本区域

    // === 图像预处理阶段 ===
    Mat gray, erod, blur;
    cvtColor(img, gray, COLOR_BGR2GRAY);  // 转换为灰度图

    // 形态学处理：腐蚀操作
    int erodeSize = gray.cols   / 300;    // 动态计算核尺寸
    erodeSize = erodeSize % 2 == 0 ? erodeSize + 1 : erodeSize;
    Mat element = getStructuringElement(MORPH_RECT, Size(erodeSize, erodeSize));
    erode(gray, erod, element);

    // 高斯模糊降噪
    int blurSize = gray.cols  / 200;
    if (blurSize % 2 == 0) blurSize++;
    GaussianBlur(erod, blur, Size(blurSize, blurSize), 0, 0);

    // 自适应阈值二值化
    Mat thresh = gray.clone();
    adaptiveThreshold(~gray, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

    // === 表格线检测阶段 ===
    int scale = 20;  // 比例因子
    Mat horizontal = thresh.clone();
    Mat vertical = thresh.clone();

    // 水平线检测
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal.cols/scale,   1));
    erode(horizontal, horizontal, horizontalStructure);
    dilate(horizontal, horizontal, horizontalStructure);

    // 垂直线检测
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical.rows/scale));
    erode(vertical, vertical, verticalStructure);
    dilate(vertical, vertical, verticalStructure);

    // test
    // cv::namedWindow("horizontal", cv::WINDOW_NORMAL);
    // cv::imshow("horizontal", horizontal);
    // cv::namedWindow("vertical", cv::WINDOW_NORMAL);
    // cv::imshow("vertical", vertical);

    // === 网格特征提取 ===
    Mat mask = horizontal + vertical;          // 合并所有线段
    Mat joints;                                // 交叉点检测
    bitwise_and(horizontal, vertical, joints); // 水平线与垂直线交集

    // 调试显示
    //cv::namedWindow("joints", cv::WINDOW_NORMAL);
    //imshow("joints", joints);

    // === 轮廓分析阶段 ===
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Mat> rois;

    // 遍历所有轮廓
    for (size_t i = 0; i < contours.size();  i++)
    {
        double area = contourArea(contours[i]);
        // 面积过滤小轮廓
        if (area < 100) continue;

        // 多边形逼近
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        // 获取边界矩形
        boundRect[i] = boundingRect(Mat(contours_poly[i]));

        // 提取交叉点区域
        Mat roi = joints(boundRect[i]);
        vector<vector<Point> > joints_contours;
        findContours(roi, joints_contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        // 合格区域需包含至少4个交叉点
        if (joints_contours.size()  <= 4) continue;

        // 保存候选区域
        rois.push_back(img(boundRect[i]).clone());
    }

    // 清理无效矩形区域
    vector<Rect>::iterator it;
    for(it = boundRect.begin();  it != boundRect.end();  ){
        if(it->width == 0 && it->height == 0)
            it = boundRect.erase(it);
        else
            ++it;
    }

    // === 网格结构分析 ===
    // 1. 提取所有交点坐标
    std::vector<cv::Point> jointPoints;
    for (int y = 0; y < joints.rows;  y++) {
        for (int x = 0; x < joints.cols;  x++) {
            if (joints.at<uchar>(y,  x) > 0) {
                jointPoints.emplace_back(x,  y);
            }
        }
    }

    // 2. 坐标聚类（合并邻近点）
    const int CLUSTER_THRESHOLD = 10; // 像素距离阈值
    auto clusterPoints = [](std::vector<int>& coords) {
        std::sort(coords.begin(),  coords.end());
        std::vector<int> clustered;
        if (coords.empty())  return clustered;

        int current = coords[0];
        clustered.push_back(current);

        for (int i = 1; i < coords.size();  i++) {
            if (coords[i] - current > CLUSTER_THRESHOLD) {
                current = coords[i];
                clustered.push_back(current);
            }
        }
        return clustered;
    };

    // 3. 分离并聚类X/Y坐标
    std::vector<int> xCoords, yCoords;
    for (const auto& pt : jointPoints) {
        xCoords.push_back(pt.x);
        yCoords.push_back(pt.y);
    }

    std::vector<int> uniqueXs = clusterPoints(xCoords);
    std::vector<int> uniqueYs = clusterPoints(yCoords);

    // 4. 构建网格映射
    int rows = uniqueYs.size()  - 1;
    int cols = uniqueXs.size()  - 1;

    // 文本对象处理
    struct TextObject {
        cv::Rect bbox;
        std::string text;
    };
    std::vector<TextObject> textObjs;
    for (const auto& obj : textInfo["text_objects"]) {
        std::vector<cv::Point> polygon;
        for (const auto& pt : obj["box"]) {
            polygon.emplace_back(pt[0],  pt[1]);
        }
        textObjs.push_back({cv::boundingRect(polygon),
                            obj["text"].get<std::string>()});
    }

    // 单元格内容容器
    struct CellContent {
        std::vector<TextObject> texts;
    };
    std::vector<std::vector<CellContent>> cells(rows,
                                                std::vector<CellContent>(cols));

    // 文本分配到单元格
    for (const auto& text : textObjs) {
        const cv::Point center(text.bbox.x  + text.bbox.width/2,
                               text.bbox.y  + text.bbox.height/2);

        // 寻找对应列
        int colIdx = -1;
        for (size_t c=0; c < uniqueXs.size()-1;  ++c) {
            if (center.x >= uniqueXs[c] && center.x < uniqueXs[c+1]) {
                colIdx = c;
                break;
            }
        }

        // 寻找对应行
        int rowIdx = -1;
        for (size_t r=0; r < uniqueYs.size()-1;  ++r) {
            if (center.y >= uniqueYs[r] && center.y < uniqueYs[r+1]) {
                rowIdx = r;
                break;
            }
        }

        if (rowIdx >=0 && colIdx >=0)
            cells[rowIdx][colIdx].texts.push_back(text);
    }

    // 多行文本排序合并
    auto requiresNoSpace = [](char last, char first) {
        // 中文、日文、韩文字符判断
        if ((last & 0x80) || (first & 0x80)) return true;
        // 标点符号判断
        const std::string punct = ",.;:?!)]}~$";
        return punct.find(last)  != std::string::npos;
    };

    json result;
    result["grid_size"] = {rows, cols};
    json cellArray = json::array();

    for (int r=0; r<rows; ++r) {
        for (int c=0; c<cols; ++c) {
            auto& cell = cells[r][c];
            std::string mergedText;

            // 多文本排序
            std::sort(cell.texts.begin(),  cell.texts.end(),
                      [](const TextObject& a, const TextObject& b) {
                          const int vertThreshold = a.bbox.height  / 2;
                          if (std::abs(a.bbox.y  - b.bbox.y)  > vertThreshold)
                              return a.bbox.y  < b.bbox.y;
                          return a.bbox.x  < b.bbox.x;
                      });

            // 智能合并
            for (size_t i=0; i<cell.texts.size();  ++i) {
                if (!mergedText.empty()  &&
                    !requiresNoSpace(mergedText.back(),
                                     cell.texts[i].text.front()))  {
                    mergedText += " ";
                }
                mergedText += cell.texts[i].text;
            }

            // 构建输出
            json cellJson;
            cellJson["row"] = r;
            cellJson["col"] = c;
            cellJson["value"] = mergedText;
            cellArray.push_back(cellJson);
        }
    }

    result["cells"] = cellArray;
    return result;
}
