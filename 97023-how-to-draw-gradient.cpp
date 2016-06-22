//http://answers.opencv.org/question/97023
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

int main( int argc, char** argv )
{
    //Set linear gradient (255 gray levels)
    Mat lines(255, 255, CV_8U, Scalar(0));
    for (int r = 0; r < lines.rows; r++)
    {
        lines.row(r).setTo(r);
    }
    namedWindow("Linear Gradient", CV_WINDOW_NORMAL);
    imshow("Linear Gradient", lines);

    //Convert to polar (needs WARP_INVERSE_MAP flag)
    cv::linearPolar(lines, lines, cv::Point(lines.cols / 2, lines.rows / 2), 255, INTER_CUBIC | WARP_FILL_OUTLIERS | WARP_INVERSE_MAP);
    namedWindow("Polar Gradient", CV_WINDOW_NORMAL);
    imshow("Polar Gradient", lines);

    //Mask out circle section
    Mat mask(lines.size(), CV_8U, Scalar(0));
    circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), lines.rows/2, Scalar(255), -1,LINE_8);
    Mat circle_gradient;
    lines.copyTo(circle_gradient, mask);
    namedWindow("Circle Gradient", CV_WINDOW_NORMAL);
    imshow("Circle Gradient", circle_gradient);
    waitKey(0);
    return 0;
}
