
#include "qa.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

#if QA_MULTI_DEMO
int main( int argc, char** argv )
{
    so32584486( argc, argv );
}
#endif


bool SmoothEdgeSingleChannel( Mat mInput,Mat &mOutput, double amount, double radius, int Threshold)
{
    if(mInput.empty())
    {
        return 0;
    }
    if(radius<1)
        radius=1;

    Mat mGSmooth,mDiff,mAbsDiff;
    mOutput = Mat(mInput.size(),mInput.type());

    GaussianBlur(mInput,mGSmooth,Size(0,0),radius);

    subtract(mGSmooth,mInput,mDiff);

    threshold(abs(2* mDiff),mAbsDiff,Threshold,255,THRESH_BINARY_INV);

    mDiff.setTo(Scalar(0),mAbsDiff);

    add(mInput,mDiff,mOutput);

    return true;
}
bool SmoothEdge( Mat mInput_Bgr,Mat &mOutput_Bgr, double amount, double radius, int Threshold)
{
    if(mInput_Bgr.empty())
    {
        return 0;
    }

    if(radius<1)
        radius=1;

    Mat mInput,mOutput;
    Mat mChannel[3];

    split(mInput_Bgr,mChannel);

    for (int i = 0; i < 3; i++)
    {
        mInput= mChannel[i];
        SmoothEdgeSingleChannel(mInput,mOutput,amount, radius,Threshold);
        mOutput.copyTo(mChannel[i]);
    }
    merge(mChannel,3,mOutput_Bgr);


    return true;
}

int so32584486(int argc, char** argv)
{
    char* filename = argc >= 2 ? argv[1] : (char*)"fruits.jpg";

    double m_Amount=0.5;
    double m_Radius=5.5;
    int m_Threshold=0;

    Mat mSource_Bgr,mSmoothEdge;
    mSource_Bgr= imread(filename,1);

    SmoothEdge(mSource_Bgr,mSmoothEdge,m_Amount,m_Radius,m_Threshold);

    imshow("Source Image",mSource_Bgr);
    imshow("Output Image",mSmoothEdge);
    waitKey(0);

}

/*
Mat mEllipse_Bgr(Size(640,480),CV_8UC3,Scalar(0));
Mat mEllipseMask(mEllipse_Bgr.size(),CV_8UC1,Scalar(0));

// Draw a ellipse
ellipse( mEllipse_Bgr, Point( 200, 200 ), Size( 100.0, 160.0 ), 45, 0, 360, Scalar( 255, 0, 0 ), 1, 8 );
ellipse( mEllipseMask, Point( 200, 200 ), Size( 100.0, 160.0 ), 45, 0, 360, Scalar( 255), -1, 8 );
imshow("Ellipse Image",mEllipse_Bgr);
imshow("Ellipse Mask",mEllipseMask);

// Perform the distance transform algorithm
Mat mDist;
distanceTransform(mEllipseMask, mDist, CV_DIST_L2, 3);

// Normalize the distance Transform image for range = {0.0, 1.0} to view it
normalize(mDist, mDist, 0, 1., NORM_MINMAX);
imshow("Distance Transform Image", mDist);
*/
