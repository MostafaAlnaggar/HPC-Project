#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat OMPHighPassFilter(const Mat& inputImage, int kernelSize, int num_threads);