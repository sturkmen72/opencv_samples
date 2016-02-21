#include <iostream>
#include <ctime>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char**argv)
{
    cv::VideoCapture capture;

    // Read the video stream or open the web cam
    if( argc > 1)
        capture.open( argv[1] );
    else
        capture.open( 0 );

    if (!capture.isOpened())
    {
        std::cout << "Problem connecting to cam " << std::endl;
        return -1;
    }
    else
    if( argc == 1 )
    {
        std::cout << "Successfuly connected to camera " << std::endl;
        //capture.set(cv::CAP_PROP_FRAME_WIDTH,320);
        //capture.set(cv::CAP_PROP_FRAME_HEIGHT,240);
    }

    int frameCounter = 0;
    int tick = 0;
    int fps;
    std::time_t timeBegin = std::time(0);

    cv::Mat frame;

    while (1)
    {
        capture.read(frame);

        if( frame.empty() )
        {
            break;
        }

        frameCounter++;

        std::time_t timeNow = std::time(0) - timeBegin;

        if (timeNow - tick >= 1)
        {
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }

        cv::putText(frame, cv::format("Average FPS=%d", fps ), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        cv::imshow("FPS test", frame);
        cv::waitKey(1);
    }

    return 0;
}
