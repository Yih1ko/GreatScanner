#ifndef CVUTILS_H
#define CVUTILS_H
#include <opencv2/opencv.hpp>
#include "json.hpp"
#include <algorithm>
using json = nlohmann::json;
using namespace cv;using namespace std;
class CvUtils
{
public:
    class tableStructure{
    public:
        json operator()(const json& textInfo){
            return exportToJson(textInfo["image_path"], textInfo);
        }
    private:
        struct CellData {
            int row;
            int col;
            int x;
            int y;
            int width;
            int height;
        };
        cv::Mat eraseRegions(cv::Mat& image, const json& data);
        json exportToJson(const std::string& imgPath, const json& textInfo);
    };
    class objectCounter{
    private:
        enum class ObjectType {
            regular,//常规形状
            stick,//棍状：牙签等
            pipe,//钢管
        };
        struct ObjectInfo{
            std::vector<std::pair<int, int>> positionMap;//id(int)->pos(int, int) id对应位置
            std::vector<float> areaMap;//id(int)->area_size(float) id对应面积
        };
        ObjectInfo& regularCounter(ObjectInfo& res, cv::Mat& image){

        }
        ObjectInfo& stickCounter(ObjectInfo& res, cv::Mat& image){

        }
        ObjectInfo& pipeCounter(ObjectInfo& res, cv::Mat& image){

        }
    public:
        // ObjectInfo operator()(const std::string& imgPath, ObjectType type){
        //     ObjectInfo result;
        //     if(type == ObjectType::regular){
        //         regularCounter(result,);
        //     }else if(type == ObjectType::stick){
        //         stickCounter(result,);
        //     }else if(type == ObjectType::pipe){
        //         pipeCounter(result,);
        //     }
        //     return result;
        // }
    };
    CvUtils(){}
    ~CvUtils(){}
};

#endif // CVUTILS_H
