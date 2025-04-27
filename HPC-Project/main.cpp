#include <sstream>
#include <iomanip>
#include <direct.h>  

#include "serial_code.h"
#include "usingOMP.h"


/// Function to get current timestamp
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;

    // Using localtime_s for Windows
    struct tm timeinfo;
    localtime_s(&timeinfo, &in_time_t);
    ss << put_time(&timeinfo, "%Y%m%d_%H%M%S");

    return ss.str();
}

// Cross-platform directory creation function
bool createDirectory(const string& path) {
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
}


int main() {
    // to disable logging
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);

    string inputFilename = "lena.png";

    // Load image in BGR color (OpenCV default) or RGB
    Mat image = imread(inputFilename, IMREAD_COLOR);  // Changed from IMREAD_GRAYSCALE
    if (image.empty()) {
        cerr << "Could not open the image!" << endl;
        return -1;
    }
    cout << "Image has " << image.channels() << " channels." << endl;

    // input from user
    int num_threads = 8;
    int kernel_size = 3;

    // Apply high-pass filter
    //Mat sharpenedImage = serialHighPassFilterRGB(image, kernel_size);
    Mat sharpenedImage = OMPHighPassFilter(image, kernel_size, num_threads);

    // Create output directory if it doesn't exist
    string outputDir = "sharpened_images";
    createDirectory(outputDir);

    // Extract original filename without extension
    size_t lastDot = inputFilename.find_last_of(".");
    string filenameWithoutExt = (lastDot != string::npos) ?
        inputFilename.substr(0, lastDot) :
        inputFilename;
    string extension = (lastDot != string::npos) ?
        inputFilename.substr(lastDot) :
        ".png";

    // Create output filename with timestamp
    string timestamp = getCurrentTimestamp();
    string outputFilename = outputDir + "/" + filenameWithoutExt + "_sharpened_" + timestamp + extension;

    // Display and save results
    imshow("Original", image);
    imshow("Sharpened", sharpenedImage);

    // Save the image
    if (imwrite(outputFilename, sharpenedImage)) {
        cout << "Image saved successfully: " << outputFilename << endl;
    }
    else {
        cerr << "Failed to save image: " << outputFilename << endl;
    }

    waitKey(0);
    return 0;
}
