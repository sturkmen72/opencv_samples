#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src,gray,small,srcclone;

static void detect(Mat src)
{
    Mat tmp,thr;
    cvtColor(src,tmp,CV_BGR2GRAY);
    threshold(tmp,thr,200,255,THRESH_BINARY_INV);

    vector< vector <Point> > contours;
    vector< Vec4i > hierarchy;
    Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));//Ceate Mat to draw contour

    int box_w=10; // Define box width here
    int box_h=10; // Define box height here
    int threshold_perc=25; //perceantage value for eliminating the box according to pixel count inside the box
    int threshold=(box_w*box_h*threshold_perc)/100;

    findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); //Find contour

    for( int i = 0; i< contours.size(); i++ )
    {
        drawContours( dst,contours, i, Scalar(255,255,255),CV_FILLED, 8, hierarchy ); // Draw contour with  thickness = filled
        Rect r= boundingRect(contours[i]); // Find bounding rect

        // Scan the image with in bounding box
        for(int j=r.x; j<r.x+r.width; j=j+box_w)
        {
            for(int k=r.y; k<r.y+r.height; k=k+box_h)
            {
                Rect roi_rect(j,k,box_w,box_h);
                Mat roi = dst(roi_rect);
                int count = countNonZero(roi);
                if(count > threshold)
                    rectangle(src, roi_rect, Scalar(255,0,0),1,8,0 );
            }
        }
    }
    imshow("result",src);

}


int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"25912.png";
    src = imread(filename);
    if(src.empty())
    {
        cout << "Could not read input image file: " << endl;
        return -1;
    }

    imshow("source",src);
    detect(src);

    moveWindow("source",40,40);
    moveWindow("result",560,40);

    waitKey();

    return 0;
}
