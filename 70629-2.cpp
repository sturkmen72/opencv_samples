//Second trial code gives this result image

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

// calculates distance between two points
static double distanceBtwPoints(const cv::Point a, const cv::Point b)
{
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;

    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

static int findNearestPointIndex(const cv::Point pt, const vector<Point> points)
{
    int nearestpointindex = 0;
    double distance;
    double mindistance = 1e+9;

    for ( size_t i = 0; i < points.size(); i++)
    {
        distance = distanceBtwPoints(pt,points[i]);

        if( distance < mindistance )
        {
            mindistance =  distance;
            nearestpointindex = i;
        }
    }
    return nearestpointindex;
}

int main( int argc, char** argv )
{
    Point pt0;
    int shift=0; // optional value for drawing scaled
    Scalar color = Scalar(0,0,0);

    char* filename = argc >= 2 ? argv[1] : (char*)"test.png";
    Mat img = imread(filename);
    if (img.empty())
        return -1;

    Mat src = img;
    Mat bw;

    cvtColor( src, bw, COLOR_BGR2GRAY );
    bw = bw < 127;

    // Find contours
    vector<vector<Point> > contours;
    vector<Point> contour;
    findContours( bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    // assuming the closed contour have biggest size
    contour = contours[0];
    for ( size_t i = 0; i < contours.size(); i++)
    {
        if( contourArea(contour) < contourArea(contours[i]) )
        {
            contour = contours[i];
        }

    }

    for ( size_t i = 0; i < contours.size(); i++)
    {
        if( contour != contours[i] && contours[i].size() > 10 )
        {
            for ( size_t j = 0; j <  contours[i].size(); j++)
            {
                pt0 = contours[i][j];
                line(src,pt0,contour[findNearestPointIndex(pt0,contour)],color,1,LINE_8,shift);
            }
        }
    }

    cvtColor( src, src, COLOR_BGR2GRAY );
    src = src < 127;
    vector<vector<Point> > contours1;
    findContours( src, contours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    src = 255; // clears source image to redraw
    for ( size_t i = 0; i < contours1.size(); i++)
    {
        drawContours(src,contours1,i,color);
    }

    imwrite("result1.png",src);
    Mat m_xor(src.size(),CV_8UC1,255);
    polylines(m_xor,contour,false,color,1,LINE_8);

    m_xor = 255 - (src ^ m_xor) ;

    imwrite("result2.png",m_xor);
    imshow("result2", m_xor);
    waitKey(0);
    return 0;
}
