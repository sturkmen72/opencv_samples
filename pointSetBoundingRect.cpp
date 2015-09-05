#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;


static Rect pointSetBoundingRect( const Mat& points , Mat m)
{
    int npoints = points.checkVector(2);


    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;
    Point ptxmin , ptymin , ptxmax , ptymax;

    if( npoints == 0 )
        return Rect();

    const Point* pts = points.ptr<Point>();
    Point pt = pts[0];

    ptxmin = ptymin = ptxmax = ptymax = pt;
    xmin = xmax = pt.x;
    ymin = ymax = pt.y;

    for( i = 1; i < npoints; i++ )
    {
        pt = pts[i];

        if( xmin > pt.x )
        {
            xmin = pt.x;
            ptxmin = pt;
        }


        if( xmax < pt.x )
        {
            xmax = pt.x;
            ptxmax = pt;
        }

        if( ymin > pt.y )
        {
            ymin = pt.y;
            ptymin = pt;
        }

        if( ymax < pt.y )
        {
            ymax = pt.y;
            ptymax = pt;
        }
    }
    ellipse( m, ptxmin, Size( 3, 3), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
    ellipse( m, ptxmax, Size( 3, 3), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
    ellipse( m, ptymin, Size( 3, 3), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
    ellipse( m, ptymax, Size( 3, 3), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
    return Rect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
}


#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    apointSetBoundingRect( argc, argv );
}
#endif

int apointSetBoundingRect(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"24463.jpg";
    Mat src=imread( filename, 0 );

    Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

    src = src > 127;
    namedWindow( "Source", 1 );
    imshow( "Source", src );

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( src, contours, hierarchy,
                  CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE );


    Rect minRect;

    if (contours.size()>0)
    {

        for( size_t i = 0; i<contours.size() ; i++ )
        {
            Scalar color( rand()&255, rand()&255, rand()&255 );
            drawContours( dst, contours, i, color, FILLED, 8, hierarchy );

            minRect = pointSetBoundingRect( Mat(contours[i]),dst );
            rectangle( dst, minRect, Scalar(rand()&255,rand()&255,rand()&255), 1, 8 );
            imshow( "contours", dst );

        }

    }

    imshow( "contours", dst );

    moveWindow( "Source", 40, 40 );
    moveWindow( "contours", 550, 40 );

    waitKey(0);

    return 0;
}
