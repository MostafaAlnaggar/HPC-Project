#include <omp.h>
#include "serial_code.h"
#include "helper_funnctions.h"
#include <chrono>



// High-pass filter using dynamic kernel
Mat serialHighPassFilter(const Mat& inputImage, int kernelSize) {
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

    for (int y = padding; y < paddedImage.rows - padding; ++y) {
        for (int x = padding; x < paddedImage.cols - padding; ++x) {
            int result = applyKernelAtPixel(paddedImage, kernel, x, y, padding);
            outputImage.at<uchar>(y - padding, x - padding) = static_cast<uchar>(result);
        }
    }

    // End timing
    auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "Execution time for serial code: " << duration.count() << " milliseconds" << endl;

    return outputImage;
}
