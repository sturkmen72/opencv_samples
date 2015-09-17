
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

/// Global Variables
Point pt0,pt1;
int shift=0; // optional value for drawing scaled
Scalar color = Scalar(0,0,0);

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
// draws point vector on given image
int static drawPoints( vector<Point> source_points,int start_index,int end_index, Mat m )
{
    vector<Point> result;
    int c = start_index;
    start_index = min( start_index , end_index );
    end_index = max( end_index, c );

    for ( int i = start_index; i < end_index; i++)
    {
        result.push_back(source_points[i]);
    }
    if( start_index == 0 || result.size() < source_points.size()/2)
    {
        polylines(m,result,false,color,1,LINE_8,shift);
    }

    else
    {
        result.clear();
        for ( int i = 0; i < start_index; i++)
            result.push_back(source_points[i]);
        polylines(m,result,false,color,1,LINE_8,shift);

        result.clear();
        for ( int i = end_index; i < source_points.size(); i++)
        {
            result.push_back(source_points[i]);
            polylines(m,result,false,color,1,LINE_8,shift);
        }
    }
    return 0;
}

bool testPoints(vector<Point> pts)
{
    Point pt = pts[0];
    for ( size_t i = 1; i < pts.size(); i++)
        if( pts[i] == pt ) return false;
    return true;
}

int main( int argc, char** argv )
{
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


    // assuming the closed contour have biggest area
    contour = contours[0];
    for ( size_t i = 0; i < contours.size(); i++)
    {
        if( contourArea(contour) < contourArea(contours[i]) )
            contour = contours[i];
    }

    src = Scalar(255,255,255); // clears source image to redraw

    for ( size_t i = 0; i < contours.size(); i++)
    {
        if( contour != contours[i] && contours[i].size() > 10 )
        {
            pt0 = contours[i][0];

            int ptscount=0;
            for ( size_t j = 0; j <  contours[i].size(); j++)
                if( contours[i][j] == contours[i][j-2] )
                {
                    ptscount++;
                    pt1 = pt0;
                    pt0 = contours[i][j-1];
                }

            if(testPoints(contours[i]) && ptscount > 1 )
                pt0 = contours[i][0];

            // find nearest points of closed contour
            int nearestpointindex_start = findNearestPointIndex(pt0,contour);
            int nearestpointindex_end = findNearestPointIndex(pt1,contour);

            // redrawing
            line(src,pt1,contour[nearestpointindex_end],color,1,LINE_8,shift);
            line(src,pt0,contour[nearestpointindex_start],color,1,LINE_8,shift);

            drawPoints(contours[i],0,contours[i].size(),src);
            drawPoints(contour,nearestpointindex_start,nearestpointindex_end,src);

        }
    }
    imwrite("result.png",src);
    imshow("result", src);

drawPoints(contour,0,contour.size(),src);
    cvtColor( src, bw, COLOR_BGR2GRAY );
    bw = bw < 127;
    findContours( bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    src = Scalar(255,255,255); // clears source image to redraw
drawPoints(contours[0],0,contours[0].size(),src);
     imwrite("result2.png",src);
    imshow("result2", src);
    waitKey(0);
    return 0;
}
