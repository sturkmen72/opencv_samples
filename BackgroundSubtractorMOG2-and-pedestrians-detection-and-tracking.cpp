
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include "opencv2/video/background_segm.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"768x576.avi";
    VideoCapture capture( filename );

    HOGDescriptor hog;
    hog.setSVMDetector(hog.getDefaultPeopleDetector());

    Ptr<BackgroundSubtractor> bgS = createBackgroundSubtractorMOG2();
    Mat frame,output;

    while(true)
    {
        capture.read(frame);
        if (!frame.data)
            return 0;
        if( frame.cols > 800 )
            resize( frame, frame, Size(), 0.5, 0.5 );

        bgS->apply(frame, output);
        erode(output,output,Mat());

        // Find contours
        vector<vector<Point> > contours;
        findContours( output, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );

        for ( size_t i = 0; i < contours.size(); i++)
        {
            Rect r = boundingRect( contours[i] );
            if( r.height > 80 & r.width < r.height )
            {
                r.x -= r.width / 2;
                r.y -= r.height / 2;
                r.width += r.width;
                r.height += r.height;
                r = r & Rect( 0, 0, frame.cols, frame.rows );

                Mat roi;
                cvtColor( frame( r ), roi, COLOR_BGR2GRAY);

                std::vector<Rect> rects;

                if( roi.cols > hog.winSize.width & roi.rows > hog.winSize.height )
                    hog.detectMultiScale( roi, rects);

                for (size_t i=0; i<rects.size(); i++)
                {
                    rects[i].x += r.x;
                    rects[i].y += r.y;

                    rectangle( frame, Point(rects[i].x, rects[i].y),
                               Point(rects[i].x+rects[i].width, rects[i].y+rects[i].height),
                               Scalar( 0, 0, 255 ), 2 );
                }
            }
        }

        imshow("display", frame);
        if(waitKey(1)==27)
        {
            break;
        }
    }
    return 0;
}
