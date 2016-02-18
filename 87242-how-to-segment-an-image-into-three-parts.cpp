
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

int main( int argc, char** argv )
{
    String imageName("lena.jpg"); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }

    Mat image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    Mat gray;
    cvtColor( image, gray, COLOR_BGR2GRAY);

    gray = gray / 128;
    gray = gray * 128;

    imshow( "source", image );
    imshow( "result", gray );
    waitKey(0); // Wait for a keystroke in the window

    return 0;
}
