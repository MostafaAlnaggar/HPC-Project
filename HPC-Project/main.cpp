#include "serial_code.h"
#include "usingOMP.h"


int main() {
    // to diasble logging
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);

    // Load a grayscale image
    Mat image = imread("lena.png", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Could not open the image!" << endl;
        return -1;
    }

    // input from user
    int num_threads = 8;
    int kernel_size = 3;

    // Apply high-pass filter
    Mat sharpenedImage = OMPHighPassFilter(image, kernel_size, num_threads);
    //Mat sharpenedImage = serialHighPassFilter(image, kernel_size);

    // Display and save results
    imshow("Original", image);
    imshow("Sharpened", sharpenedImage);
    imwrite("sharpened_output.jpg", sharpenedImage);
    waitKey(0);

    return 0;
}
