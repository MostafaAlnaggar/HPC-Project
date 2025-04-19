#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat serialHighPassFilter(const Mat& inputImage, int kernelSize);