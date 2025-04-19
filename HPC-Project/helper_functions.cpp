#include "helper_funnctions.h"

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