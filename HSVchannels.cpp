#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
Mat src= imread( argv[1] );
Mat imgHSV,channels[3];
cvtColor( src, imgHSV, COLOR_BGR2HSV );
split( imgHSV, channels);

imshow( "channels[0]", channels[0] );
imshow( "channels[1]", channels[1] );
imshow( "channels[2]", channels[2] );
imshow( "imgHSV", imgHSV );

Mat m16uc1;
channels[2].convertTo(m16uc1,CV_16SC1);
imshow( "m16uc1", m16uc1 );
imwrite("m16uc1.png", m16uc1);
waitKey();

return 0;
}
