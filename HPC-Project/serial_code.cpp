#include <omp.h>
#include "serial_code.h"
#include "helper_funnctions.h"



// High-pass filter using dynamic kernel
Mat serialHighPassFilter(const Mat& inputImage, int kernelSize) {
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

    // Start timing
    double start = omp_get_wtime();


    for (int y = padding; y < paddedImage.rows - padding; ++y) {
        for (int x = padding; x < paddedImage.cols - padding; ++x) {
            int result = applyKernelAtPixel(paddedImage, kernel, x, y, padding);
            outputImage.at<uchar>(y - padding, x - padding) = static_cast<uchar>(result);
        }
    }

    // End timing
    double end = omp_get_wtime();
    cout << "Execution time for serial code: " << (end - start) << " seconds" << endl;

    return outputImage;
}
