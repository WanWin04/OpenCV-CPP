#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;

// Sobel function to compute gradients in x and y directions
void Sobel(const Mat& gray, Mat& I_x, Mat& I_y) {
    int rows = gray.rows;
    int cols = gray.cols;
    I_x = Mat::zeros(rows, cols, CV_32F);
    I_y = Mat::zeros(rows, cols, CV_32F);

    // Sobel kernels for gradient computation
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Apply Sobel kernels to compute gradients
    for (int y = 1; y < rows - 1; ++y) {
        for (int x = 1; x < cols - 1; ++x) {
            float gradient_x = 0, gradient_y = 0;

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    float pixel = (float)gray.at<uchar>(y + i, x + j);
                    gradient_x += Gx[i + 1][j + 1] * pixel;
                    gradient_y += Gy[i + 1][j + 1] * pixel;
                }
            }

            I_x.at<float>(y, x) = gradient_x;
            I_y.at<float>(y, x) = gradient_y;
        }
    }
}

// Harris function to compute the Harris response matrix R
void Harris(const Mat& I_x, const Mat& I_y, Mat& R, float k = 0.04, int windowSize = 5) {
    int rows = I_x.rows, cols = I_y.cols;
    R = Mat::zeros(rows, cols, CV_32F);

    // Compute products of gradients
    Mat I_xx = I_x.mul(I_x);
    Mat I_yy = I_y.mul(I_y);
    Mat I_xy = I_x.mul(I_y);

    // Apply Gaussian blur to smooth the gradient products
    GaussianBlur(I_xx, I_xx, Size(windowSize, windowSize), 0);
    GaussianBlur(I_yy, I_yy, Size(windowSize, windowSize), 0);
    GaussianBlur(I_xy, I_xy, Size(windowSize, windowSize), 0);

    // Compute Harris response for each pixel
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            float a = I_xx.at<float>(y, x);
            float b = I_xy.at<float>(y, x);
            float c = I_yy.at<float>(y, x);

            float detM = (a * c) - (b * b);
            float traceM = a + c;
            R.at<float>(y, x) = detM - k * (traceM * traceM);
        }
    }
}

// Function to detect corners based on the Harris response matrix
vector<Point> detector(const Mat& R, float threshold, int maxCorners = 500) {
    vector<Point> corners;
    int rows = R.rows, cols = R.cols;
    
    Mat R_norm;
    normalize(R, R_norm, 0, 1, NORM_MINMAX);
    
    vector<pair<float, Point>> cornerResponses;
    
    // Find local maxima in the Harris response matrix
    for (int y = 2; y < rows - 2; ++y) {
        for (int x = 2; x < cols - 2; ++x) {
            float value = R_norm.at<float>(y, x);
            
            if (value > threshold) {
                bool isMax = true;
                for (int dy = -1; dy <= 1 && isMax; ++dy) {
                    for (int dx = -1; dx <= 1 && isMax; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (R_norm.at<float>(y + dy, x + dx) > value) {
                            isMax = false;
                        }
                    }
                }
                
                if (isMax) {
                    cornerResponses.push_back(make_pair(value, Point(x, y)));
                }
            }
        }
    }
    
    // Sort corners by response strength and select the top ones
    sort(cornerResponses.begin(), cornerResponses.end(), 
         [](const pair<float, Point>& a, const pair<float, Point>& b) {
             return a.first > b.first;
         });
    
    int numCorners = min((int)cornerResponses.size(), maxCorners);
    for (int i = 0; i < numCorners; ++i) {
        corners.push_back(cornerResponses[i].second);
    }
    
    return corners;
}

// Function to filter corners to ensure a minimum distance between them
vector<Point> filterCorners(const vector<Point>& corners, int minDistance) {
    if (corners.empty()) return corners;
    
    vector<Point> filteredCorners;
    vector<bool> isCornerValid(corners.size(), true);
    
    for (size_t i = 0; i < corners.size(); ++i) {
        if (!isCornerValid[i]) continue;
        
        filteredCorners.push_back(corners[i]);
        
        for (size_t j = i + 1; j < corners.size(); ++j) {
            if (!isCornerValid[j]) continue;
            
            double dist = norm(Mat(corners[i]), Mat(corners[j]));
            if (dist < minDistance) {
                isCornerValid[j] = false;
            }
        }
    }
    
    return filteredCorners;
}

// Main function to perform Harris corner detection on an image
void harrisCornerDetection(const string& inputPath, const string& outputPath) {
    Mat img = imread(inputPath);
    if (img.empty()) {
        cout << "Error loading image: " << inputPath << endl;
        return;
    }
    
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    
    Mat I_x, I_y;
    Sobel(gray, I_x, I_y);
    
    Mat R;
    Harris(I_x, I_y, R, 0.04f, 5); // k=0.04, window size=5
    
    float threshold = 0.01f;
    int maxCorners = 500;
    vector<Point> corners = detector(R, threshold, maxCorners);
    
    int minDistance = 10;
    vector<Point> filteredCorners = filterCorners(corners, minDistance);
    
    Mat result = img.clone();
    for (const auto& corner : filteredCorners) {
        circle(result, corner, 4, Scalar(0, 255, 0), 2);
    }
    
    Mat R_viz;
    normalize(R, R_viz, 0, 255, NORM_MINMAX);
    R_viz.convertTo(R_viz, CV_8U);
    applyColorMap(R_viz, R_viz, COLORMAP_JET);
    
    imshow("Harris Corners", result);
    imwrite(outputPath, result);
    
    waitKey(0);
}

int main(int argc, char** argv) {
    if (argc != 4 || string(argv[1]) != "-harris") {
        cout << "Usage: " << argv[0] << " -harris <InputFilePath> <OutputFilePath>" << endl;
        return -1;
    }

    string inputPath = argv[2];
    string outputPath = argv[3];

    harrisCornerDetection(inputPath, outputPath);

    return 0;
}