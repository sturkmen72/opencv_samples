//modified from https://github.com/Itseez/opencv/blob/master/apps/annotation/opencv_annotation.cpp
// move the mouse on the window and try left click and right click
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

// Function prototypes
void on_mouse(int, int, int, int, void*);


// Public parameters
Mat image(600, 800, CV_8UC3, Scalar(220, 220, 220));
vector<Point> points;
int roi_x0 = 0, roi_y0 = 0, roi_x1 = 0, roi_y1 = 0;
bool start_draw = false;

// Window name for visualisation purposes
const string window_name = "OpenCV Mouse Event Demo";

// FUNCTION : Mouse response for selecting objects in images
// If left button is clicked, start drawing a rectangle and circle as long as mouse moves
//If right button is clicked, look what will happen !
 // Stop drawing once a new left click is detected by the on_mouse function
void on_mouse(int event, int x, int y, int, void *)
{
    // Action when right button is clicked
    if (event == EVENT_RBUTTONDOWN)
    {
        points.push_back(Point(x, y));
        circle(image, Point(x, y), 5, Scalar(255, 255, 255), 1, 8);


        if (points.size() == 3)
        {
            // Find the minimum area enclosing circle
            Point2f center, vtx[4];
            float radius = 0;
            minEnclosingCircle(Mat(points), center, radius);

            circle(image, center, cvRound(radius), Scalar(0, 255, 255), 1, LINE_AA);
            points.clear();
        }

        imshow(window_name, image);
    }

    // Action when left button is clicked
    if (event == EVENT_LBUTTONDOWN)
    {
        if (!start_draw)
        {
            roi_x0 = x;
            roi_y0 = y;
            start_draw = true;
        }
        else {
            roi_x1 = x;
            roi_y1 = y;
            start_draw = false;
        }
    }
    // Action when mouse is moving
    if ((event == EVENT_MOUSEMOVE) && start_draw)
    {
        // Redraw bounding box and rectangle
        Mat current_view;
        image.copyTo(current_view);
        rectangle(current_view, Point(roi_x0, roi_y0), Point(x, y), Scalar(0, 0, 255));
        int radius = max(abs(roi_x0 - x), abs(roi_y0 - y));
        circle(current_view, Point(roi_x0, roi_y0), radius, Scalar(255, 0, 0), 1, 8);
        imshow(window_name, current_view);
    }
}


int main(int argc, const char** argv)
{
    // Init window interface and couple mouse actions
    namedWindow(window_name, WINDOW_AUTOSIZE);
    setMouseCallback(window_name, on_mouse);

    imshow(window_name, image);

    int key_pressed = 0;

    do
    {
        // Keys for processing
        // Based on the universal ASCII code of the keystroke: http://www.asciitable.com/
        //      c = 99          add rectangle to current image
        //      <ESC> = 27      exit program
        key_pressed = 0xFF & waitKey(0);
        if (key_pressed==99)
        {
            // draw a rectangle and a circle on the image

            int radius = max(abs(roi_x0 - roi_x1), abs(roi_y0 - roi_y1));
            circle(image, Point(roi_x0, roi_y0), radius, Scalar(0, 0, 255), 1, 8);
            rectangle(image, Point(roi_x0, roi_y0), Point(roi_x1, roi_y1), Scalar(255, 0, 0), 1);
            imshow(window_name, image);
        }
    }
    // Continue as long as the <ESC> key has not been pressed
    while (key_pressed != 27);

    // Close down the window
    destroyWindow(window_name);
    return 0;
}
