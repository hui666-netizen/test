#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    // 读取图像
    Mat image = imread(argv[1]);
    if (image.empty()) {
        cout << "Error: Image cannot be loaded..!!" << endl;
        return -1;
    }

    // 转换为HSV颜色空间
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // 颜色过滤：提取绿色区域
    Mat mask;
    inRange(hsv, Scalar(35, 100, 100), Scalar(85, 255, 255), mask);  // 绿色范围

    // 形态学操作：闭操作和开操作
    Mat morph;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(mask, morph, MORPH_CLOSE, kernel);
    morphologyEx(morph, morph, MORPH_OPEN, kernel);

    // 边缘检测
    Mat edges;
    Canny(morph, edges, 100, 50);

    // 轮廓检测
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // 绘制轮廓的边框
    for (size_t i = 0; i < contours.size(); i++) {
        // 计算轮廓的边界框
        Rect boundingBox = boundingRect(contours[i]);

        // 筛选条件：根据边界框的宽高比和面积等条件筛选
        double aspectRatio = (double)boundingBox.width / boundingBox.height;
        double area = contourArea(contours[i]);

        // 示例筛选条件：面积大于100，宽高比在0.8到1.2之间
        if (area > 500 && aspectRatio > 0.8 && aspectRatio < 1.2) {
            // 绘制轮廓的边框
            rectangle(image, boundingBox, Scalar(0, 0, 255), 2);
        }
    }

    // 显示结果
    imshow("Sign Detection", image);
    waitKey(0);

    return 0;
}