#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

int main()
{
    Mat3b bgr = imread("gnfXj.jpg");

    Mat3b ycrcb;
    cvtColor(bgr, ycrcb, COLOR_BGR2YCrCb);

    vector<Mat1b> planes;
    split(ycrcb, planes);

    // Collage planes
    Mat1b collagePlanes(bgr.rows, bgr.cols*3);
    for (int i = 0; i < 3; ++i)
    {
        planes[i].copyTo(collagePlanes(Rect(i*bgr.cols, 0, bgr.cols, bgr.rows)));
    }

    Mat1b gray(bgr.rows, bgr.cols, uchar(128));

    // Y
    vector<Mat1b> vy(3);
    vy[0] = planes[0];
    vy[1] = gray.clone();
    vy[2] = gray.clone();
    Mat3b my;
    merge(vy, my);

    // Cr
    vector<Mat1b> vcr(3);
    vcr[0] = gray.clone();
    vcr[1] = planes[1];
    vcr[2] = gray.clone();
    Mat3b mcr;
    merge(vcr, mcr);

    // Cb
    vector<Mat1b> vcb(3);
    vcb[0] = gray.clone();
    vcb[1] = gray.clone();
    vcb[2] = planes[2];
    Mat3b mcb;
    merge(vcb, mcb);



    // Collage planes
    Mat3b collageColor(bgr.rows, bgr.cols * 3);
    my.copyTo(collageColor(Rect(0, 0, bgr.cols, bgr.rows)));
    mcr.copyTo(collageColor(Rect(bgr.cols, 0, bgr.cols, bgr.rows)));
    mcb.copyTo(collageColor(Rect(2 * bgr.cols, 0, bgr.cols, bgr.rows)));

    cvtColor(collageColor, collageColor, COLOR_YCrCb2BGR);


    ////////////////////////////

    // Blur Y
    boxFilter(planes[0], planes[0], CV_8U, Size(7,7));

    Mat3b blurred;
    merge(planes, blurred);
    cvtColor(blurred, blurred, COLOR_YCrCb2BGR);


    imshow("Original", bgr);
    imshow("YCrCb planes", collagePlanes);
    imshow("YCrCb planes colored", collageColor);
    imshow("Blurred", blurred);
    waitKey();


    return 0;
}
