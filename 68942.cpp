#include "qa.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
   a68942( argc, argv );
}
#endif

int a68942( int argc, char** argv )
{

String InterpolationFlags[] = {"INTER_NEAREST","INTER_LINEAR","INTER_CUBIC","INTER_AREA","INTER_LANCZOS4"};


for( int i=0; i < 5; i++)
{
char* filename = argc >= 2 ? argv[1] : (char*)"fruits.jpg";

Mat img = imread( filename );

Mat roi1 =img(Rect( 0, 0, img.cols/2, img.rows/2)).clone();
Mat roi2 =img(Rect( img.cols/2, 0, img.cols/2, img.rows/2)).clone();
Mat roi3 =img(Rect( 0, img.rows/2, img.cols/2, img.rows/2)).clone();
Mat roi4 =img(Rect( img.cols/2, img.rows/2, img.cols/2, img.rows/2)).clone();

resize( img, img, Size( img.cols/2, img.rows/2 ),0,0, i);

resize( roi1, roi1, Size( roi1.cols/2, roi1.rows/2 ),0,0, i);
resize( roi2, roi2, Size( roi2.cols/2, roi2.rows/2 ),0,0, i);
resize( roi3, roi3, Size( roi3.cols/2, roi3.rows/2 ),0,0, i);
resize( roi4, roi4, Size( roi4.cols/2, roi4.rows/2 ),0,0, i);

Mat rroi1 =img(Rect( 0, 0, img.cols/2, img.rows/2));
Mat rroi2 =img(Rect( img.cols/2, 0, img.cols/2, img.rows/2));
Mat rroi3 =img(Rect( 0, img.rows/2, img.cols/2, img.rows/2));
Mat rroi4 =img(Rect( img.cols/2, img.rows/2, img.cols/2, img.rows/2));

rroi1 = rroi1 - roi1;
rroi2 = rroi2 - roi2;
rroi3 = rroi3 - roi3;
rroi4 = rroi4 - roi4;

img = img > 0;
imshow(InterpolationFlags[i],img);
moveWindow(InterpolationFlags[i],i*240,20);
//imshow("roi1",roi1);
//imshow("roi2",roi2);
//imshow("roi3",roi3);
//imshow("roi4",roi4);

}
waitKey(0);
return 0;

}
