#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv)
{
    VideoCapture cap;

    if(!cap.open(0))
        return 0;

    Mat frame[5];

    for(int i=0; i<5; )
    {

        cap >> frame[i];

        if( !frame[i].empty() )
        {
            imshow(format("Frame - %d",i), frame[i]);
            i++;
        }
    }

    waitKey();
    return 0;
}
