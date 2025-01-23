#include "stdafx.h"

cv::Mat correlation(const cv::Mat& image, const cv::Mat& kernel) {
    int height = image.rows, width = image.cols;
    int kh = kernel.rows, kw = kernel.cols;
    cv::Mat output(height - kh + 1, width - kw + 1, CV_32F, cv::Scalar(0));

    for (int i = 0; i <= height - kh; ++i) {
        for (int j = 0; j <= width - kw; ++j) {
            float sum = 0.0;
            for (int u = 0; u < kh; ++u) {
                for (int v = 0; v < kw; ++v) {
                    sum += image.at<uchar>(i + u, j + v) * kernel.at<float>(u, v);
                }
            }
            output.at<float>(i, j) = sum;
        }
    }
    return output;
}

cv::Mat convolution(const cv::Mat& image, const cv::Mat& kernel) {
    cv::Mat flipped_kernel;
    flip(kernel, flipped_kernel, -1);
    return correlation(image, flipped_kernel);
}

int main() {
    cv::Mat image = cv::imread("E:/CV01/image/lena.jpg", cv::IMREAD_GRAYSCALE);

    cv::Mat top_left_region = image(cv::Rect(0, 0, 20, 20));

    // Kernel Sobel
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 
                        1,  0, -1,
                        2,  0, -2,
                        1,  0, -1);

    cv::Mat correlation_result = correlation(top_left_region, kernel);
    cv::Mat convolution_result = convolution(top_left_region, kernel);

    cv::Mat opencv_correlation, opencv_convolution;
    filter2D(top_left_region, opencv_correlation, CV_32F, kernel);
    filter2D(top_left_region, opencv_convolution, CV_32F, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    cv::normalize(correlation_result, correlation_result, 0, 255, cv::NORM_MINMAX);
    correlation_result.convertTo(correlation_result, CV_8U);

    cv::normalize(convolution_result, convolution_result, 0, 255, cv::NORM_MINMAX);
    convolution_result.convertTo(convolution_result, CV_8U);

    cv::normalize(opencv_correlation, opencv_correlation, 0, 255, cv::NORM_MINMAX);
    opencv_correlation.convertTo(opencv_correlation, CV_8U);

    cv::normalize(opencv_convolution, opencv_convolution, 0, 255, cv::NORM_MINMAX);
    opencv_convolution.convertTo(opencv_convolution, CV_8U);

    imshow("Original 20x20 Region", top_left_region);
    imshow("Correlation Result (Manual)", correlation_result);
    imshow("Convolution Result (Manual)", convolution_result);
    imshow("OpenCV Correlation", opencv_correlation);
    imshow("OpenCV Convolution", opencv_convolution);

    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}
