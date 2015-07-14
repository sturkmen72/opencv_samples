// experimental code
// http://answers.opencv.org/question/65284


#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

Mat src,gray,small,srcclone;
int threshval = 25;

static void on_trackbar(int, void*)
{

    Mat bw = threshval < 128 ? (gray < threshval) : (gray > threshval);
    //imshow( "threshold", bw );

    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;

    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    srcclone=src.clone();
    Rect minRect;

    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Calculate the area of each contour
        double area = contourArea(contours[i]);
        // Ignore contours that are too small or too large
        // if (area < 1e2 || 1e5 < area) continue;

        minRect = boundingRect( Mat(contours[i]) );

        if (minRect.height < 20 ) continue;

        Scalar color= Scalar(0,255,0);

        if (minRect.height < 50 | minRect.height > minRect.width*2) color= Scalar(0,0,255);

        rectangle( srcclone, minRect,color , 2, 8 );

        // Draw each contour only for visualisation purposes
        //  drawContours(srcclone, contours, static_cast<int>(i), Scalar(0, 0, 255), 2, 8, hierarchy, 0);
    }

    imwrite("result-65284.jpg",srcclone);
    resize(srcclone,small,Size(src.cols/2,src.rows/2)); // resize image to fit display

    imshow( "Display Window", small );
}


int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"65284.png";
    src = imread(filename);
    if(src.empty())
        return -1;

    cvtColor( src, gray, COLOR_BGR2GRAY );


// you can try different values below
    GaussianBlur(gray,gray,Size(5,5),1,1);
    Mat kernel = Mat::ones(3, 3, CV_8UC1);
    dilate(gray, gray, kernel);


    namedWindow( "Display Window", 1 );
    createTrackbar( "Threshold", "Display Window", &threshval, 127, on_trackbar );
    on_trackbar(threshval, 0);

    waitKey(0);
    return 0;
}
