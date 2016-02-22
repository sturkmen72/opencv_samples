#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"fingers-1.jpg";
    Mat src = imread( filename );
    if (src.empty())
        return -1;

    Mat bw;
    cvtColor( src, bw, COLOR_BGR2GRAY );
    bw = bw > 60;

    // Find contours
    vector<vector<Point> > contours;
    vector<int> contoursHull;
    vector<Vec4i> defects;
    findContours( bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    Point pStart = Point();

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

                if( pStart == Point())
                {
                    pStart =  contours[i][defpoint[0]];
                }

                circle(src,contours[i][defpoint[0]],3,Scalar(255,0,0),-1);
                circle(src,contours[i][defpoint[1]],3,Scalar(0,255,0),-1);
                circle(src,contours[i][defpoint[2]],3,Scalar(255,0,255),-1);

                line( src, pStart, contours[i][defpoint[0]], Scalar( 0, 0, 255), 2 );
                line( src, contours[i][defpoint[0]], contours[i][defpoint[2]], Scalar( 0, 0, 255), 2 );
                line( src, contours[i][defpoint[2]], contours[i][defpoint[1]], Scalar( 0, 0, 255), 2 );

                pStart =  contours[i][defpoint[1]];
            }
        }
    }
    imshow("result", src);
    waitKey();
    return 0;
}
