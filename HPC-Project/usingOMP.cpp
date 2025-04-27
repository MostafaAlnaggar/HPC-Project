#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <omp.h>

#include "helper_functions.h"



// High-pass filter using dynamic kernel
Mat OMPHighPassFilter(const Mat& inputImage, int kernelSize, int num_threads) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        cerr << "Kernel size must be odd and >= 3." << endl;
        return inputImage.clone();
    }

    int padding = (kernelSize - 1) / 2;
    vector<vector<int>> kernel = generateKernel(kernelSize);

	printKernel(kernel);

    // Pad input image
    Mat paddedImage;
    copyMakeBorder(inputImage, paddedImage, padding, padding, padding, padding, BORDER_REPLICATE);


    // Prepare output
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // Start timing
    int start = clock();

    int x, y, result;

    // Parallelize the outer loop using OpenMP
#pragma omp parallel for \
    num_threads(num_threads) \
    default(none) \
    shared(paddedImage, kernel, outputImage, padding) \
    private(x, y, result) \
    schedule(static, 1)
    for (y = padding; y < paddedImage.rows - padding; y++) {
        for (x = padding; x < paddedImage.cols - padding; x++) {
            Vec3b result;
            for (int c = 0; c < 3; c++) {  // Process B, G, R channels
                int channelResult = applyKernelAtPixelRGB(paddedImage, kernel, x, y, padding, c);
                result[c] = static_cast<uchar>(channelResult);
            }
            outputImage.at<Vec3b>(y - padding, x - padding) = result;
        }
    }

    // End timing
    int stop = clock();
    int TotalTime = (stop - start) / double(CLOCKS_PER_SEC) * 1000;
    cout << "Execution time for omp code: " << TotalTime << " milliseconds" << endl;

    return outputImage;
}



