#include <omp.h>
#include "serial_code.h"
#include "helper_functions.h"
#include <chrono>



// High-pass filter using dynamic kernel
Mat serialHighPassFilter(const Mat& inputImage, int kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        cerr << "Kernel size must be odd and >= 3." << endl;
        return inputImage;
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

    for (int y = padding; y < paddedImage.rows - padding; y++) {
        for (int x = padding; x < paddedImage.cols - padding; x++) {
            int result = applyKernelAtPixel(paddedImage, kernel, x, y, padding);
            outputImage.at<uchar>(y - padding, x - padding) = static_cast<uchar>(result);
        }
    }

    // End timing
    int stop = clock();
    int TotalTime = (stop - start) / double(CLOCKS_PER_SEC) * 1000;

	cout << "Execution time for serial code: " << TotalTime << " milliseconds" << endl;

    return outputImage;
}


Mat serialHighPassFilterRGB(const Mat& inputImage, int kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        cerr << "Kernel size must be odd and >= 3." << endl;
        return inputImage;
    }

    int padding = (kernelSize - 1) / 2;
    vector<vector<int>> kernel = generateKernel(kernelSize);
    printKernel(kernel);

    // Pad input image
    Mat paddedImage;
    copyMakeBorder(inputImage, paddedImage, padding, padding, padding, padding, BORDER_REPLICATE);

    // Prepare output (same type as input - 3 channels)
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // Start timing
    int start = clock();

    for (int y = padding; y < paddedImage.rows - padding; y++) {
        for (int x = padding; x < paddedImage.cols - padding; x++) {
            // Process each channel separately
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
    cout << "Execution time for serial code: " << TotalTime << " milliseconds" << endl;
    return outputImage;
}
