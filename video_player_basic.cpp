#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <windows.h>

using namespace cv;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"D:\\opencv\\samples\\data\\768x576.avi";

    VideoCapture capture( filename ); // open video file

    int fps_of_video = (int) capture.get( CAP_PROP_FPS );
    int time_to_wait = 1000 / fps_of_video;

    for(;;)
    {
        double time_start = (double)getTickCount();

        Mat frame;
        capture >> frame; // get a new frame

        if( frame.empty() )
        {
            break;
        }

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
