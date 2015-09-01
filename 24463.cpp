#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void test(Mat src)
{
    Mat dst;//(src.rows,src.cols,CV_8UC4);
    Mat tmp,alpha;

    cvtColor(src,tmp,CV_BGR2GRAY);
    threshold(tmp,alpha,100,255,THRESH_BINARY);

    Mat rgb[3];
    split(src,rgb);

    Mat rgba[4]= {rgb[0],rgb[1],rgb[2],alpha};
    merge(rgba,4,dst);
    if( imwrite("result-24463.png",dst) )
        cout << "result-24463.png saved!" ;

}

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    a24463( argc, argv );
}
#endif

int a24463(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"24463.jpg";
    Mat src = imread( filename ,1 ),gray,temp;

    imshow("source",src);

    test(src);

    moveWindow("Source",50,50);
    waitKey(0);

    return 0;
}
