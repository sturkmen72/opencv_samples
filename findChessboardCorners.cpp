#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

using namespace cv;
using namespace std;

//Sample usage of detecting and drawing chessboard corners

bool detectAndDrawChessboardCorners()
{
    Mat img = imread("right07.jpg",33);
    imshow("image",img);
    moveWindow("image",40,40);

    Size patternsize(8,6); //interior number of corners
    Mat gray;
    cvtColor(img,gray,COLOR_BGR2GRAY);//source image
    vector<Point2f> corners; //this will be filled by the detected corners

    //CALIB_CB_FAST_CHECK saves a lot of time on images
    //that do not contain any chessboard corners
    bool patternfound = findChessboardCorners(gray, patternsize, corners,
                        CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                        + CALIB_CB_FAST_CHECK);

    if(patternfound)
        cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                     TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

    drawChessboardCorners(img, patternsize, Mat(corners), patternfound);

    imshow("result",img);
    moveWindow("result",img.cols/2,100);
    waitKey(0);
    return true;
}

//Sample usage of detecting and drawing the centers of circles

bool detectAndDrawCentersOfCircles()
{
    Mat img = imread("circles6.png");
    imshow("image",img);
    Size patternsize(7,7); //number of centers
    Mat gray;
    cvtColor(img,gray,COLOR_BGR2GRAY);//source image
    vector<Point2f> centers; //this will be filled by the detected centers

    bool patternfound = findCirclesGrid(gray, patternsize, centers);

    drawChessboardCorners(img, patternsize, Mat(centers), patternfound);

    imshow("result",img);

    waitKey(0);
    return true;
}

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    detectAndDrawChessboardCorners();
    detectAndDrawCentersOfCircles();
}
#endif

