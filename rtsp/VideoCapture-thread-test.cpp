#include "opencv2//imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;

cv::VideoCapture capture;
bool capture_frames;
bool do_detection = false;
cv::Mat captured_frame;
const cv::String CASCADE_FILE("haarcascade_frontalface_default.xml");

void *CaptureImages( void *threadid )
{
    if (!capture.isOpened())
    {
        pthread_exit(NULL);
    }
    capture_frames = true;
    while ( capture_frames && capture.isOpened())
    {
        capture >> captured_frame;
        if( captured_frame.empty() )
        {
            break;
        }
    }
    captured_frame.release();
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    capture.open( argv[1] );
    if (!capture.isOpened())
    {
        return -1;
    }
    capture >> captured_frame;

    pthread_t capture_thread;
    int thread_id=0;
    int rc = pthread_create(&capture_thread, NULL,
                            CaptureImages, (void *)thread_id );
    if (rc)
    {
        return -1;
    }

    cv::CascadeClassifier faceCascade;
    faceCascade.load( CASCADE_FILE );
    int frameCounter = 0;
    int tick = 0;
    int fps;
    std::time_t timeBegin = std::time(0);
    int scale = 8;

    while ( capture_frames )
    {
        std::vector<cv::Rect> allFaces;

        if( ( frameCounter % 3 == 0 ) & do_detection & !faceCascade.empty() )
        {
            cv::Mat resized;
            cv::resize( captured_frame, resized, cv::Size(), (float) 1/scale, (float) 1/scale );

            faceCascade.detectMultiScale( resized, allFaces );
        }

        for( size_t i = 0; i < allFaces.size(); i++ )
        {
            allFaces[i].x *= scale;
            allFaces[i].y *= scale;
            allFaces[i].width *= scale;
            allFaces[i].height *= scale;
            cv::rectangle( captured_frame, allFaces[i], cv::Scalar( 0, 255, 0 ), 2 );
        }
        frameCounter++;

        std::time_t timeNow = std::time(0) - timeBegin;

        if (timeNow - tick >= 1)
        {
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }
        cv::putText(captured_frame, cv::format("Average FPS=%d (1/%dx)", fps, scale ), cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        cv::imshow("Video Capture", captured_frame);
        int key = cv::waitKey(30);
        if( key == 27 ) capture_frames = false;
        if( key == 32 ) do_detection = !do_detection;
        if( key == 13 ) scale++;
        if( scale > 10 ) scale = 2;
    }
    return 0;
}
