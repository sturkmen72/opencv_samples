#include "qa.hpp"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "stdio.h"
#include "string"
#include <stdlib.h>

using namespace cv;
using namespace std;
RNG rng(12345);

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    a59782( argc, argv );
}
#endif

int a59782( int argc, char** argv )
{
    cvNamedWindow( "RGB", 1 );
    //cvNamedWindow( "HSV", 1 );
    //cvNamedWindow( "Binary", 1 );
    //cvNamedWindow( "Binary1", 1 );
    cvNamedWindow( "Contour", 1 );
    //cvNamedWindow( "Final", 1 );
    Mat img,hsv,binary,binary1,imgToProcess;

    char* filename = argc >= 2 ? argv[1] : (char*)"59782.png";

    img = imread( filename, 1 );
    imshow("RGB",img);

    //convert RGB image into HSV image
    cvtColor(img, hsv, CV_BGR2HSV);
    //imshow("HSV",hsv);

    //get binary image
    inRange(hsv, Scalar(0, 85, 241), Scalar(18, 255, 255), binary);
    // imshow("Binary",binary);
    inRange(hsv, Scalar(171, 0, 0), Scalar(255, 255, 255), binary1);

    //binary.copyTo(binary1);
    //imshow("Binary1",binary1);
    add(binary1, binary, imgToProcess, noArray(), 8);

    //absdiff(binary1, binary, imgToProcess);
    imshow("Binary2",imgToProcess);


    //find contours from binary image
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imgToProcess, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); //find contours

    vector<vector<Point> > contours_poly( contours.size() );
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<float>radius( contours.size() );
    vector<float>area( contours.size() );
    vector<Point2f>center( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( contours_poly[i], center[i], radius[i] );
        area[i]= contourArea(Mat(contours_poly[i]));
    }


    Mat drawing = Mat::zeros( imgToProcess.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        ellipse( drawing, minEllipse[i], color, 2, 8 );// ellipse
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
        //Point2f rect_points[4]; minRect[i].points( rect_points );
        //for( int j = 0; j < 4; j++ )
        //   line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

    }
    imshow("Contour",drawing);


    //wait for key press
    waitKey();
    return 0;
}
