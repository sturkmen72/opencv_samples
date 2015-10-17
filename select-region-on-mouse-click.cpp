//http://opencvexamples.blogspot.com/2014/01/select-region-on-mouse-click.html

#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;
using namespace std;

int var1=1;
Point pos;
int radius = 7;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        pos.x=x;
        pos.y=y;
        var1=0;
    }
}


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow("Video",1);
    int area = (2*radius+1)*(2*radius+1);

    Mat frame;

    //select the region on the image
    while(var1==1)
    {
        //set the callback function for any mouse event
        setMouseCallback("Video", CallBackFunc, NULL);

        cap >> frame;         // get a new frame from camera
        //show the image
        imshow("Video", frame);

        // Wait until user press some key
        waitKey(10);
    }

    GaussianBlur( frame, frame, Size( 15, 15 ), 0, 0 );imshow("blur",frame);
    Mat imgHSV,out;
    cvtColor(frame, imgHSV, CV_BGR2HSV);    imshow("HSV",imgHSV);
    Mat final;
    final = frame;

    float blue=0, green=0,red=0, b_var=0, g_var=0, r_var=0;
    for(int var2=pos.y-radius;var2<=pos.y+radius;var2++)
        for(int var3=pos.x-radius;var3<=pos.x+radius;var3++)
        {
            Vec3b intensity = final.at<Vec3b>(var2,var3);
            blue = blue + intensity.val[0];
            green = green + intensity.val[1];
            red = red + intensity.val[2];
        }
    red=red/area; green=green/area; blue=blue/area;

    for(int var2=pos.y-radius;var2<=pos.y+radius;var2++)
        for(int var3=pos.x-radius;var3<=pos.x+radius;var3++)
        {
            Vec3b intensity = final.at<Vec3b>(var2,var3);
            b_var = b_var + (intensity.val[0]-blue)*(intensity.val[0]-blue);
            g_var = g_var + (intensity.val[1]-green)*(intensity.val[1]-green);
            r_var = r_var + (intensity.val[2]-red)*(intensity.val[2]-red);
        }

    r_var/=area; g_var/=area; b_var/=area;
    r_var=sqrt(r_var); g_var=sqrt(g_var); b_var=sqrt(b_var);
    cout<<red<<" "<<green<<"  "<<blue<<" "<<r_var<<" "<<g_var<<" "<<b_var;



    inRange(final,Scalar(blue-b_var,green-g_var,red-r_var),Scalar(blue+b_var,green+g_var,red+r_var),out);imshow("out",out);

    waitKey(0);
    return 0;
}
