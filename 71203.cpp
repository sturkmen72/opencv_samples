#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

bool selectObject = false;
Rect selection;
Point origin;
int msize = 5;
Mat image,blurredImage;

vector<Rect> blurredAreas;

static bool doMosaic( Mat img, int msize)
{
    for (int i = 0; i < img.cols-msize; i+=msize)
        for(int j = 0; j < img.rows-msize; j+=msize)
        {
            Rect r = Rect(i,j,msize,msize);
            Mat mosaic = img( r );
            mosaic.setTo(mean(mosaic));
        }
    return true;
}

static bool doBlur()
{
    for(size_t i = 0; i< blurredAreas.size(); i++)
    {
        Mat roi = blurredImage(selection);
        //GaussianBlur(roi,roi,Size(),5,5);
        doMosaic(roi,msize);
    }
    imshow("Demo", blurredImage);
    return true;
}



static void onMouse(int event, int x, int y, int, void*)
{
    switch (event)
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x, y);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
    {
        selectObject = false;
        blurredAreas.push_back(selection);
        doBlur();
        break;
    }
    }

    if (selectObject)
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x)+1;
        selection.height = std::abs(y - origin.y)+1;
        selection &= Rect(0, 0, image.cols, image.rows);

        if ( selection.width > 0 && selection.height > 0)
        {
            Mat blurredImagecopy;
            blurredImage.copyTo(blurredImagecopy);
            Mat roi = blurredImagecopy(selection);
            bitwise_not(roi, roi);
            imshow("Demo", blurredImagecopy);
        }
    }
}


int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"fruits.jpg";
    image = imread(filename);
    image.copyTo(blurredImage);

    namedWindow("Demo");
    setMouseCallback("Demo", onMouse );

    imshow("Demo", image);

    while( true )
    {
        int key = waitKey(0);

        if( key == 27 )
            break;

        if( key == 's' ) // saves result image
        {
          imwrite("result.jpg",blurredImage);
        }

        if( key == 'i' ) // space key for clear blurred areas
        {
            msize +=5;
            image.copyTo(blurredImage);
            doBlur();
        }

        if( key == 'd' ) // space key for clear blurred areas
        {
            msize = msize == 5 ? 5 : msize - 5;
            image.copyTo(blurredImage);
            doBlur();
        }

        if( key == 32 ) // space key for clear blurred areas
        {
            blurredAreas.clear();
            image.copyTo(blurredImage);
            doBlur();
        }
    }
    return 0;
}
