#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"fingers-1.png";
    Mat src = imread( filename );
    if (src.empty())
        return -1;

    Mat bw;
    cvtColor( src, bw, CV_BGR2GRAY );
    bw = bw > 127;

    // Find contours
    vector<vector<Point> > contours;
    findContours( bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    fillPoly(src,contours,Scalar(0,255,255));

    for ( size_t i = 0; i < contours.size(); i++)
    {
    drawContours(src,contours,i,Scalar(0,0,255),3);
    //approxPolyDP(contours[i],contours[i],5,true);
    convexHull(contours[i],contours[i]);
    fillConvexPoly(src,contours[i],Scalar(255,0,0));
    }

    imshow("result", src);
    waitKey();
    return 0;
}
