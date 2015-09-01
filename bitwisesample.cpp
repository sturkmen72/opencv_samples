#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    bitwisesample( argc,  argv );
}
#endif

int bitwisesample( int argc, char** argv )
{
    // Load two images
    Mat img1 = imread( "fruits.jpg" );
    Mat img2 = imread( "opencv-logo.png" );

    imshow( "image1", img1 );
    imshow( "image2", img2 );

    pyrUp( img1, img1 );

    // I want to put logo on center, So I create a ROI
    Mat roi =img1( Rect(( img1.cols - img2.cols ) / 2, ( img1.rows - img2.rows ) / 2, img2.cols, img2.rows ) );

    // Now create a mask of logo and create its inverse mask also
    Mat img2gray, mask, mask_inv, img1_bg, img2_fg;
    cvtColor( img2, img2gray, COLOR_BGR2GRAY );

    threshold(img2gray,mask, 200, 255, THRESH_BINARY_INV );
    bitwise_not(mask,mask_inv);

    // Take only region of logo from logo image.
    bitwise_and( img2, img2, img2_fg, mask = mask );

    // Now black-out the area of logo in ROI
    bitwise_and( roi, roi, img1_bg, mask = mask_inv );

    // Put logo in ROI and modify the main image
    add( img1_bg, img2_fg, roi );

    pyrDown( img1, img1 );
    imwrite( "result.jpg", img1 );
    imshow( "result", img1);

    moveWindow( "image1", 50, 50 );
    moveWindow( "image2", 100, 100 );
    moveWindow( "result", 690, 50 );

    waitKey();
    return 0;
}
