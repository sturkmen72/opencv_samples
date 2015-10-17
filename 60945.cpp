#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Mat shrinkMat(Mat src,int percent,bool clone=false)
{
    if (percent>99)
        return src;

    Rect newroi;
    newroi.width=(src.cols*percent)/100;
    newroi.height=(src.rows*percent)/100;
    newroi.x=(src.cols-newroi.width)/2;
    newroi.y=(src.rows-newroi.height)/2;

    if (clone) return src(newroi).clone();

    return src(newroi);
}

int main()
{

    Mat image(600,800,CV_8UC3,Scalar(220,220,220));
    Mat newimage;


    for (int i=2; i<99; i+=10)
    {

        newimage=shrinkMat(image,i);
        rectangle(newimage,Rect(0,0,newimage.cols,newimage.rows),Scalar(rand()&255,rand()&255,rand()&255),3);

    }

    newimage=shrinkMat(image,45);
    ellipse( newimage, Point(newimage.cols/2,newimage.rows/2), Size( newimage.cols/2, newimage.rows/2), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );


    newimage=shrinkMat(image,75,true);
    ellipse( newimage, Point(newimage.cols/2,newimage.rows/2), Size( newimage.cols/2, newimage.rows/2), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );

    imshow( "Display window1", image );
    imshow( "Display window2", newimage );

    waitKey(0);
    return 0;
}
