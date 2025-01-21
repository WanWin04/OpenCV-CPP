#include "stdafx.h"

int main() {
    cv::Mat image = cv::imread("E:/CV01/image/lena.jpg", cv::IMREAD_COLOR);

    cv::namedWindow("Show Image");

    imshow("Show Image", image);

    cv::waitKey(0);

    return 0;
}