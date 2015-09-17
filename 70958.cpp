
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        int shift = 1;

        Rect r=boundingRect( Mat(squares[i]));
        r.x = r.x + r.width / 4;
        r.y = r.y + r.height / 4;
        r.width = r.width / 2;
        r.height = r.height / 2;

        Mat roi = image(r);
        Scalar color = mean(roi);

        fillConvexPoly(image,Mat( squares[i] ),color,1,shift);
    }
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares , bool inv=false)
{
    squares.clear();

    Mat gray,gray0;

    vector<vector<Point> > contours;

    cvtColor(image,gray0,COLOR_BGR2GRAY);
    GaussianBlur(gray0, gray0, Size(7,7), 1.5, 1.5);
    Canny(gray0,gray, 0, 30, 3);

    // find contours and store them all as a list
    findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<Point> approx;

    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, 9, true);

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 && contourArea(Mat(approx)) > image.cols )
        {
            double maxCosine = 0;

            for( int j = 2; j < 5; j++ )
            {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if( maxCosine < 0.3 )
                squares.push_back(approx);
        }
    }

//for( size_t i = 0; i < squares.size(); i++ )


}

int main()
{
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        return -1;
    }

    Mat frame;
    vector<vector<Point> > squares;

    for (;;)
    {
        cap >> frame;

        if (frame.empty())
        {
            return -1;
        }
        findSquares(frame, squares);
        drawSquares(frame, squares);
        imshow("Rubic Detection Demo", frame);
        waitKey(1);
    }
    return 0;
}
