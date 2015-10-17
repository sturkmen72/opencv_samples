#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main( int, char** argv )
{
    Mat image(400, 400, CV_8UC4, Scalar(0, 0, 200));
    Mat rgba[4];
    split(image,rgba);

    rectangle(rgba[3],Rect(100,100,200,200),Scalar(255),-1);

    merge(rgba,4,image);
    imwrite("transparent1.png",image);

    rectangle(rgba[3],Rect(150,150,100,100),Scalar(127),-1);
    merge(rgba,4,image);
    imwrite("transparent2.png",image);

    rgba[3] = rgba[3] * 0.5; // here you can change transparency

    merge(rgba,4,image);
    imwrite("transparent3.png",image);

    waitKey(0);
    return(0);
}
