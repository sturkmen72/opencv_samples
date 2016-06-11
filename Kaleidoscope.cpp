//https://github.com/SaranshKejriwal/Kaleidoscope

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img,img2,kal1(400,400,CV_8UC3,Scalar(0,0,0));
    Mat kal2,add;
    VideoCapture cap(0);
    int i=0,j=0;
    for(;;)
    {
        cap>>img;
        resize(img,img,Size(400,400));
        for(i=0; i<600; i++)
        {
            for(j=0; j<i/3; j++)
            {
                kal1.at<uchar>(Point(i,j))=img.at<uchar>(Point(i,j));//gives 1/8 of img

                if(j>i)
                {
                    continue;
                }
            }

        }
        flip(kal1,kal2,0);
        addWeighted(kal1,1,kal2,1,0,add);
        flip(kal2,kal2,1);
        addWeighted(add,1,kal2,1,0,add);
        flip(kal2,kal2,0);
        addWeighted(add,1,kal2,1,0,add);
        transpose(kal2,kal2);
        addWeighted(add,1,kal2,1,0,add);
        flip(kal2,kal2,0);
        addWeighted(add,1,kal2,1,0,add);
        flip(kal2,kal2,1);
        addWeighted(add,1,kal2,1,0,add);
        flip(kal2,kal2,0);
        addWeighted(add,1,kal2,1,0,add);
        line(img,Point(200,0),Point(200,200),Scalar(0,255,255),1);
        line(img,Point(0,0),Point(200,200),Scalar(0,255,255),1);
        moveWindow("img",30,30);
        moveWindow("Kaleidoscope",500,30);
        imshow("img",img);
        imshow("Kaleidoscope",add);
        waitKey(10);
    }

    return 1;

}
