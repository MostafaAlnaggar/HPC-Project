#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;



// Generate high-pass filter kernel dynamically based on kernel size
vector<vector<int>> getHighPassKernel(int kernelSize) {
    /*vector<vector<int>> kernel(kernelSize, vector<int>(kernelSize, -1));
    int center = kernelSize / 2;
    kernel[center][center] = (kernelSize * kernelSize) - 1;
    return kernel;*/

    vector<vector<int>> kernel = {
        { 0, -1,  0 },
        {-1,  4, -1 },
        { 0, -1,  0 }
    };

    return kernel;
}


// Applies kernel at a specific (x, y) location
int applyKernelAtPixel(const Mat& paddedImage, const vector<vector<int>>& kernel, int x, int y, int padding) {
    int sum = 0;
    for (int ky = -padding; ky <= padding; ++ky) {
        for (int kx = -padding; kx <= padding; ++kx) {
            int pixel = paddedImage.at<uchar>(y + ky, x + kx);
            int weight = kernel[ky + padding][kx + padding];
            sum += pixel * weight;
        }
    }

    return min(255, max(0, sum)); // Clamp
}


// High-pass filter using dynamic kernel
Mat applyHighPassFilter(const Mat& inputImage, int kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        cerr << "Kernel size must be odd and >= 3." << endl;
        return inputImage.clone();
    }

    //int padding = (kernelSize - 1) / 2;
    int padding = 1;
    auto kernel = getHighPassKernel(kernelSize);

    // Pad input image
    Mat paddedImage;
    copyMakeBorder(inputImage, paddedImage, padding, padding, padding, padding, BORDER_REPLICATE);


    // Prepare output
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    for (int y = padding; y < paddedImage.rows - padding; ++y) {
        for (int x = padding; x < paddedImage.cols - padding; ++x) {
            int result = applyKernelAtPixel(paddedImage, kernel, x, y, padding);
            outputImage.at<uchar>(y - padding, x - padding) = static_cast<uchar>(result);
        }
    }

    return outputImage;
}



int main() {
    // to diasble logging
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);

    // Load a grayscale image
    Mat image = imread("lena.png", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Could not open the image!" << endl;
        return -1;
    }

    // Apply high-pass filter
    Mat sharpenedImage = applyHighPassFilter(image, 3);

    // Display and save results
    imshow("Original", image);
    imshow("Sharpened", sharpenedImage);
    imwrite("sharpened_output.jpg", sharpenedImage);
    waitKey(0);

    return 0;
}
