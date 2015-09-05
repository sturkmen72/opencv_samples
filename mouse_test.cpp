#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

String winName = "Mouse Events Test";

String MouseEventNames[] =
{
    "EVENT_MOUSEMOVE",
    "EVENT_LBUTTONDOWN",
    "EVENT_RBUTTONDOWN",
    "EVENT_MBUTTONDOWN",
    "EVENT_LBUTTONUP",
    "EVENT_RBUTTONUP",
    "EVENT_MBUTTONUP",
    "EVENT_LBUTTONDBLCLK",
    "EVENT_RBUTTONDBLCLK",
    "EVENT_MBUTTONDBLCLK",
    "EVENT_MOUSEWHEEL",
    "EVENT_MOUSEHWHEEL"
};

String MouseEventFlagNames[] =
{
    "EVENT_FLAG_LBUTTON",
    "EVENT_FLAG_RBUTTON",
    "EVENT_FLAG_MBUTTON",
    "EVENT_FLAG_CTRLKEY",
    "EVENT_FLAG_SHIFTKEY",
    "EVENT_FLAG_ALTKEY"
};

void onMouse( int event, int x, int y, int flags, void* userdata )
{
    int i;
    Scalar color;

    Mat img = *((Mat *)userdata);
    Vec3b pixel = img.at<Vec3b>(y,x);



    if( ( event > 0 ) || ( y < 300 ) ) // disable EVENT_MOUSEMOVE event on bottom part of the window.
    {
        img.setTo( Scalar( 220, 220, 220 ) );
        color = Scalar( pixel[0], pixel[1],pixel[2] );
        putText( img, "Color of this text will change according pixel value of the mouse pointer", Point( 40,580 ) , FONT_HERSHEY_SIMPLEX, .4, color, 1 );

        color = Scalar( 255, 0, 0 );
        line( img, Point( 0, 300 ), Point( 600, 300 ), color, 2 );
        putText( img, "EVENT_MOUSEMOVE event is disabled", Point( 300,320 ) , FONT_HERSHEY_SIMPLEX, .5, color, 1 );
        putText( img, "on bottom part of the window", Point( 300,340 ) , FONT_HERSHEY_SIMPLEX, .5, color, 1 );

        for( i=0 ; i<12 ; i++ )
        {
            color = i==event ? Scalar( 0, 0, 255 ) : Scalar( 150, 150, 150 );
            putText( img, MouseEventNames[i], Point( 40, 30*i+20 ) , FONT_HERSHEY_SIMPLEX, .7, color, 2 );
        }

        for( i=0 ; i<6 ; i++ )
        {
            color = (int)pow( 2, i ) & flags ? Scalar( 0, 0, 255 ) : Scalar( 150, 150, 150 );
            putText( img, MouseEventFlagNames[i], Point( 40, 30*i+400 ) , FONT_HERSHEY_SIMPLEX, .7, color, 2 );
        }

        putText( img, format( "x : %d", x ), Point( 350,30*event+20 ) , FONT_HERSHEY_SIMPLEX, .7, Scalar( 0, 0, 255 ), 2 );
        putText( img, format( "y : %d", y ), Point( 500,30*event+20 ) , FONT_HERSHEY_SIMPLEX, .7, Scalar( 0, 0, 255 ), 2 );

        imshow( winName, img );

    }
}

int main( int argc, char**argv )
{
    Mat image( 600, 600, CV_8UC3 );
    namedWindow( winName, WINDOW_AUTOSIZE );

    setMouseCallback( winName, onMouse, &image );

    imshow( winName, image );

    waitKey();

    return 0;
}
