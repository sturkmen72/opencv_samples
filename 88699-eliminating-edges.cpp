#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"88699.png";
    Mat src = imread(filename);
    if (src.empty())
        return -1;

    Mat gray,eroded,reduced_w;

    cvtColor( src, gray, CV_BGR2GRAY );
    erode( gray, eroded, Mat::ones(8,1,CV_8UC1) ); // change of kernel gives different results

    reduce( eroded, reduced_w, 1, CV_REDUCE_AVG );

    for ( int i = 0; i < src.rows; i++)
    {
        if( reduced_w.at<uchar>(0,i) < 100)
            line( gray,Point(0,i),Point(gray.cols,i),Scalar(255,255,255),3);
    }

    vector< vector <Point> > contours;

    eroded = gray < 127;
    findContours( eroded, contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    eroded = Scalar(255);
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Rect r = boundingRect(contours[i]);
        if((contourArea( contours[i] ) > 200) & r.x > 5 & r.x + r.width < gray.cols - 5 )
        {
            rectangle( eroded, r, Scalar(0), -1 );
            rectangle( src, r, Scalar(0,0,255), 2 );
        }
    }
    gray = gray + eroded;
    imshow("result", gray );
    imshow("color result", src );
    waitKey(0);
    return 0;
}
