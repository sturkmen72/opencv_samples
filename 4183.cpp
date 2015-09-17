
#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    a4183( argc, argv );
}
#endif

int a4183(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"65164.png";
    Mat src = imread( filename ,1 ),gray,temp;

    Mat mSource_Bgr,mSource_Gray,mThreshold;
    mSource_Bgr= src;
    cvtColor(mSource_Bgr,mSource_Gray,COLOR_BGR2GRAY);
    threshold(mSource_Gray,mThreshold,254,255,THRESH_BINARY_INV);

    Mat Points;
    findNonZero(mThreshold,Points);
    Rect Min_Rect=boundingRect(Points);

    rectangle(mSource_Bgr,Min_Rect.tl(),Min_Rect.br(),Scalar(0,255,0),2);

    imshow("Result",mSource_Bgr);

    waitKey(0);

    return 0;
}
