#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera

        Mat image;
        copyMakeBorder(frame, image, 50, 0, 0, 0, BORDER_CONSTANT); // here you can widen your image

        putText(image,format("frame dimensions : %d x %d",frame.cols,frame.rows),Point(10, 20),FONT_HERSHEY_PLAIN, 1, Scalar::all(255));
        putText(image,format("image dimensions : %d x %d",image.cols,image.rows),Point(10, 40),FONT_HERSHEY_PLAIN, 1, Scalar::all(255));
        imshow("copyMakeBorder example", image);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
