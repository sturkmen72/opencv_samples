#include <iostream>
#include <opencv2/opencv.hpp>
#include "stdio.h"

#define HUEMAX 179
#define SATMAX 255
#define VALMAX 255

using namespace std;
using namespace cv;

Mat HSV;
int H =170;
int S=200;
int V =200;
int R=0;
int G=0;
int B=0;

int MAX_H=179;
int MAX_S=255;
int MAX_V=255;
int mouse_x=0;
int mouse_y=0;
char window_name[20] = "HSV Color Plot";

//Global variable for hsv color wheel plot
int max_hue_range=179;
int max_step=3; //nuber of pixel for each hue color
int wheel_width=max_hue_range*max_step;
int wheel_hight=50;
int wheel_x=50; //x-position of wheel
int wheel_y=5;//y-position of wheel

//Global variable plot for satuarion-value plot
int S_V_Width=MAX_S;
int S_V_Height=MAX_S;
int S_V_x=10;
int S_V_y=wheel_y+wheel_hight+20;

//Global variable for HSV ploat
int HSV_Width=150;
int HSV_Height=150;
int HSV_x=S_V_x+S_V_Width+30;
int HSV_y=S_V_y+50;


void onTrackbar_changed(int, void*);
static void onMouse( int event, int x, int y, int, void* );
void drawPointers(void);

int main()
{

HSV.create(390,640,CV_8UC3); //Mat to store clock image
HSV.setTo(Scalar(200,0,200));

namedWindow(window_name);
createTrackbar( "Hue",window_name, &H, HUEMAX, onTrackbar_changed );
createTrackbar( "Saturation",window_name, &S, SATMAX,onTrackbar_changed );
createTrackbar( "Value",window_name, &V, VALMAX,onTrackbar_changed);
onTrackbar_changed(0,0); //initialoze window

setMouseCallback( window_name, onMouse, 0 );
 while(true)
  {
    int c;
    c = waitKey( 20 );
    if( (char)c == 27 )
      { break; }
   }

    return 0;
}

void onTrackbar_changed(int, void*){

//Plot color wheel.
int hue_range=0;
int step=1;
for(int i=wheel_y;i<wheel_hight+wheel_y;i++){
    hue_range=0;
    for(int j=wheel_x;j<wheel_width+wheel_x;j++){
   if(hue_range>=max_hue_range) hue_range=0;
       if(step++==max_step){
           hue_range++;
           step=1;
       }
        Vec3b pix;
        pix.val[0]=hue_range;
        pix.val[1]=255;
        pix.val[2]=255;


     HSV.at<Vec3b>(i,j)=pix;

    }

}


//Plot for saturation and value
int sat_range=0;
int value_range=255;
for(int i=S_V_y;i<S_V_Height+S_V_y;i++){
    value_range--;
    sat_range=0;
for(int j=S_V_x;j<S_V_Width+S_V_x;j++){
        Vec3b pix;
        pix.val[0]=H;
        pix.val[1]=sat_range++;
        pix.val[2]=value_range;
     HSV.at<Vec3b>(i,j)=pix;

    }

}

//Ploat for HSV
Mat roi1(HSV,Rect(HSV_x,HSV_y,HSV_Width,HSV_Height));
roi1=Scalar(H,S,V);
drawPointers();

Mat RGB;
cvtColor(HSV, RGB,CV_HSV2BGR);

imshow(window_name,RGB);
imwrite("hsv.jpg",RGB);

}

static void onMouse( int event, int x, int y, int f, void* ){
if(f&CV_EVENT_FLAG_LBUTTON){
        mouse_x=x;
        mouse_y=y;
    if(((wheel_x<=x)&&(x<=wheel_x+wheel_width))&&((wheel_y<=y)&&(y<=wheel_y+wheel_hight))){
        H=(x-wheel_x)/ max_step;
        cvSetTrackbarPos("Hue", window_name, H);
        }
    else if(((S_V_x<=x)&&(x<=S_V_x+S_V_Width))&&((S_V_y<=y)&&(y<=S_V_y+S_V_Height))){

        S=x-S_V_x;
        y=y-S_V_y;
        V=255-y;

        cvSetTrackbarPos("Saturation", window_name, S);
        cvSetTrackbarPos("Value", window_name, V);


        }

}

}

void drawPointers(){
   // Point p(S_V_x+S,S_V_y+(255-V));
    Point p(S,255-V);



    int index=10;
    Point p1,p2;
    p1.x=p.x-index;
    p1.y=p.y;
    p2.x=p.x+index;
    p2.y=p.y;

    Mat roi1(HSV,Rect(S_V_x,S_V_y,S_V_Width,S_V_Height));
    line(roi1, p1, p2,Scalar(255,255,255),1,CV_AA,0);
    p1.x=p.x;
    p1.y=p.y-index;
    p2.x=p.x;
    p2.y=p.y+index;
    line(roi1, p1, p2,Scalar(255,255,255),1,CV_AA,0);

    int x_index=wheel_x+H*max_step;
    if(x_index>=wheel_x+wheel_width) x_index=wheel_x+wheel_width-2;
    if(x_index<=wheel_x) x_index=wheel_x+2;

    p1.x=x_index;
    p1.y=wheel_y+1;
    p2.x=x_index;
    p2.y=wheel_y+20;
    line(HSV, p1, p2,Scalar(255,255,255),2,CV_AA,0);

    Mat RGB(1,1,CV_8UC3);
    Mat temp;
    RGB=Scalar(H,S,V);
    cvtColor(RGB, temp,CV_HSV2BGR);
    Vec3b rgb=temp.at<Vec3b>(0,0);
    B=rgb.val[0];
    G=rgb.val[1];
    R=rgb.val[2];

    Mat roi2(HSV,Rect(450,130,175,175));
    roi2=Scalar(200,0,200);

    char name[30];
    sprintf(name,"R=%d",R);
    putText(HSV,name, Point(460,155) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );

    sprintf(name,"G=%d",G);
    putText(HSV,name, Point(460,180) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );

    sprintf(name,"B=%d",B);
    putText(HSV,name, Point(460,205) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );


    sprintf(name,"H=%d",H);
    putText(HSV,name, Point(545,155) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );

    sprintf(name,"S=%d",S);
    putText(HSV,name, Point(545,180) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );

    sprintf(name,"V=%d",V);
    putText(HSV,name, Point(545,205) , FONT_HERSHEY_SIMPLEX, .7, Scalar(5,255,255), 2,8,false );


}
