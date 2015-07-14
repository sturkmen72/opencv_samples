#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv)
{
    VideoCapture cap;

    if(!cap.open(0))
        return 0;

    Mat frame[5];

    for(int i; i<5; i++ )
    {
        cap >> frame[i];
    }

    for(int i; i<5; i++ )
    {
        imshow(format("Frame - %d",i), frame[i]);
    }

    waitKey();
    return 0;
}
