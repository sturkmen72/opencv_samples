#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <condition_variable>
#include <windows.h>

using namespace cv;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"D:\\opencv\\samples\\data\\768x576.avi";

    VideoCapture capture( filename ); // open video file
    if(!capture.isOpened())  // check if we succeeded
        return -1;

    int fps_of_video = (int) capture.get( CAP_PROP_FPS );
    int time_to_wait = 1000 / fps_of_video;

    int frameCounter = 0;
    int tick = 0;
    int fps = fps_of_video;
    std::time_t timeBegin = std::time(0);

    for(;;)
    {
        double time_start = (double)getTickCount();

        Mat frame;
        capture >> frame; // get a new frame

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

        cv::putText(frame, cv::format("Original FPS of Video=%d", fps_of_video ), cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        cv::putText(frame, cv::format("Average FPS=%d", fps ), cv::Point(30, 80), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));

        imshow( "Video Player", frame );

        if( waitKey( 1 ) == 27 ) break;

        // wait for some time to correct FPS
        while( time_to_wait > ((double)getTickCount() - time_start)/getTickFrequency() * 1000 )
        {
            //http://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
            Sleep( 2 );
        }
    }
    return 0;
}
