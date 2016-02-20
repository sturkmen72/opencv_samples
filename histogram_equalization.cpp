//https://en.wikipedia.org/wiki/Histogram_equalization
//http://answers.opencv.org/question/6013

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int /*argc*/, char** /*argv*/) {

    // wikipedia example
    Mat img = (Mat_<uchar>(8,8) <<
        52, 55, 61, 66, 70, 61, 64, 73,
        63, 59, 55, 90, 109, 85, 69, 72,
        62, 59, 68, 113, 144, 104, 66, 73,
        63, 58, 71, 122, 154, 106, 70, 69,
        67, 61, 68, 104, 126, 88, 68, 70,
        79, 65, 60, 70, 77, 68, 58, 75,
        85, 71, 64, 59, 55, 61, 65, 83,
        87, 79, 69, 68, 65, 76, 78, 94 );
    cout << "test Mat 8x8" << endl << img << endl;
    double m, M;
    minMaxLoc(img, &m, &M, 0, 0);
    cout << "min value: " << m << endl;
    cout << "max value: " << M << endl;

    Mat imgEqualized;
    equalizeHist(img, imgEqualized);
    cout << imgEqualized << endl;

    return 0;
}
