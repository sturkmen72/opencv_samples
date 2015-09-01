#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

void step2(Mat src)
{
    Mat gray;
    cvtColor(src,gray,CV_BGR2GRAY);
    threshold(gray,gray,200,255,THRESH_BINARY_INV);

    vector< vector <Point> > contours;
    vector< Vec4i > hierarchy;

    findContours( gray, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    for( size_t i = 0; i< contours.size(); i=hierarchy[i][0] )
    {
        Rect r= boundingRect(contours[i]);
        rectangle(src,Point(r.x,r.y), Point(r.x+r.width,r.y+r.height), Scalar(0,0,255),1,8,0);

    }
    imwrite("result-65061-2.jpg",src);
    imshow("step 2 result",src);

}

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    a65061( argc, argv );
}
#endif

int a65061( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"65061.png";
    Mat src = imread( filename ,1 ),gray,temp;

    cvtColor(src,gray,CV_BGR2GRAY);
    gray = gray > 127;
    imshow( "Source", src );

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( gray, contours, hierarchy,
                  CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE );

    Rect minRect;

    if (contours.size()>0)
    {
        for( size_t i = 0; i<contours.size() ; i++ )
        {
            minRect = boundingRect( Mat(contours[i]) );
            if (minRect.width>src.cols*0.9)
            {
                minRect.height=minRect.y+4;
                minRect.y=0;
                temp=src(minRect);
                temp=Scalar(255,255,255);
                imshow( "step 1 result", src );
                imwrite("result-65061-1.jpg",src);

                step2(src);
                moveWindow("Source",50,50);
                moveWindow("step 1 result",370,50);
                moveWindow("step 2 result",690,50);
                waitKey(0);
            }
        }
    }
    return 0;
}
