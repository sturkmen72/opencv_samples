#include "qa.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
   a69434( argc, argv );
}
#endif

int a69434( int argc, char** argv )
{
    Mat img = imread( "69434.png", IMREAD_ANYCOLOR );
    Mat imgHSV, imgRGB;

    cvtColor( img, imgHSV, COLOR_BGR2HSV );

    for ( int i = 0; i < imgHSV.rows; i++ )
    {
        Vec3b *ptr=(Vec3b*)imgHSV.ptr(i);

        for ( int j=0; j < imgHSV.cols; j++,ptr++ )
            if ( (*ptr)[1] < 127 )
                (*ptr)[1] *= 2;
            else
                (*ptr)[1] = 255;
    }

    cvtColor( imgHSV, imgRGB, COLOR_HSV2BGR );

    imshow( "Original", img );
    imshow( "Saturate", imgRGB );

    moveWindow( "Original", 50, 50 );
    moveWindow( "Saturate", 350, 50 );

    waitKey();
    return 0;
}
