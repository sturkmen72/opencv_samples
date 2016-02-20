#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    Mat src = imread( argv[1] );

    imshow("source",src);

    Mat gray,dst;
    cvtColor(src,gray,COLOR_BGR2GRAY);
    cvtColor(gray,dst,COLOR_GRAY2BGR);

    dst = dst - Scalar(127,127,0);
    imshow("result 1",dst);
    waitKey();
    Mat bgr[3];
    split(src,bgr);

    Mat mZero = Mat::zeros(src.size(),CV_8UC1);
    Mat bgr1[3]= {mZero,mZero,bgr[1]};
    merge(bgr1,3,dst);
    imshow("result 2",dst);

    Mat bgr2[3]= {bgr[0],mZero,bgr[1]};
    merge(bgr2,3,dst);
    imshow("result 3",dst);
    waitKey();
}
