#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <omp.h>

#include "helper_funnctions.h"



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
    auto start = chrono::high_resolution_clock::now();

    int x, y, result;

    // Parallelize the outer loop using OpenMP
#pragma omp parallel for \
    num_threads(num_threads) \
    default(none) \
    shared(paddedImage, kernel, outputImage, padding) \
    private(x, y, result) \
    schedule(static, 1)
    for (y = padding; y < paddedImage.rows - padding; ++y) {
        for (x = padding; x < paddedImage.cols - padding; ++x) {
            result = applyKernelAtPixel(paddedImage, kernel, x, y, padding);
            outputImage.at<uchar>(y - padding, x - padding) = static_cast<uchar>(result);
        }
    }

    // End timing
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time for omp code: " << duration.count() << " milliseconds" << endl;

    return outputImage;
}



