#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<vector<int>> getHighPassKernel(int kernelSize);
int applyKernelAtPixel(const Mat& paddedImage, const vector<vector<int>>& kernel, int x, int y, int padding);