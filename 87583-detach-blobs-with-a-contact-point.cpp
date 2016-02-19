#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"87583.bmp";
    Mat src = imread( filename );
    if (src.empty())
        return -1;

    Mat bw;
    cvtColor( src, bw, COLOR_BGR2GRAY );

    bw = bw < 60;

    // Find contours
    vector<vector<Point> > contours;
    vector<int> contoursHull;
    vector<Vec4i> defects;
    findContours( bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    for ( size_t i = 0; i < contours.size(); i++)
    {
        if( contourArea(contours[i]) > 500 )
        {
            approxPolyDP(contours[i],contours[i],2,true);
            convexHull(contours[i], contoursHull,true);
            convexityDefects(contours[i], contoursHull,defects);

            for ( size_t j = 0; j <  defects.size(); j++)
            {
                Vec4i defpoint = defects[j];
                Point pt = contours[i][defpoint[2]]; // get defect point
                Rect r3x3(pt.x-2, pt.y-2, 5, 5 ); // create 5x5 Rect from defect point
                r3x3 = r3x3 & Rect(0, 0, bw.cols, bw.rows ); // maybe no need but to be sure that the rect is in the image
                int non_zero_pixels = countNonZero( bw(r3x3) );
                cout << non_zero_pixels << endl;
                if( non_zero_pixels > 17 )
                    circle(src,contours[i][defpoint[2]],2,Scalar(0,255,0),1);
            }
        }
    }
    imshow("result", src);
    waitKey();
    return 0;
}
