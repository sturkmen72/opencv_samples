#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

// Global variables
const int slider_max = 100;
int slider;
Mat img;
// Callback function for trackbar event
void on_trackbar(int , void *)
{
    // here's the main gotcha:
    // int(10)/int(100) == 0 !
    // we have to cast to float, to avoid the problem with integer division !
    // also, scale should never become 0, so add a minimum value of 1:
    float scale = float(slider+1)/100;

    Mat img_converted;
    Size s(img.size().width*scale, img.size().height*scale);
    resize(img, img_converted, s);
    imshow("Scaled", img_converted);
}

int main( int argc, char** argv )
{
    img = imread(argv[1]);
    namedWindow("Trackbar app", 0);

    // better start with a nice value (not 0)
    slider = 100;

    // trick: put the trackbar in its own window,
    // it really sucks, if it has to resize along with the image !!
    createTrackbar("Size", "Trackbar app", &slider, slider_max, on_trackbar);

    // make it show without having to move the trackbar initially:
    on_trackbar(0,0);

    // just wait forever. (no loop nessecary)
    waitKey();
    return 0;
}
