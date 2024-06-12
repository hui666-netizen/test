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

    // 灰度化
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 高斯模糊
    Mat blurred;
    GaussianBlur(gray, blurred, Size(5, 5), 0);

    // 边缘检测
    Mat edges;
    Canny(blurred, edges, 50, 150);

    // 轮廓检测
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // 筛选轮廓并绘制
    Mat contour_output = Mat::zeros(edges.size(), CV_8UC3);
    int imageHeight = image.rows;
    for (size_t i = 0; i < contours.size(); i++) {
        // 计算轮廓的边界框
        Rect boundingBox = boundingRect(contours[i]);

        // 筛选条件：仅处理位于图像下半部分的轮廓
        if (boundingBox.y > imageHeight * 11 / 24) {
            double area = contourArea(contours[i]);
            double perimeter = arcLength(contours[i], true);

            // 其他筛选条件：面积、周长等
            if (area > 60 && perimeter > 20) {
                drawContours(image, contours, (int)i, Scalar(0, 255, 0), 2, LINE_AA, hierarchy, 0);
            }
        }
    }

    // 显示结果
    imshow("Lane Detection", image);
    waitKey(0);

    return 0;
}