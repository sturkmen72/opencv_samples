
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

const char* wndname = "Cross and Circle Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static double distanceBtwPoints(const cv::Point &a, const cv::Point &b)
{
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;

    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

static double sum_distanceBtwPoints( vector<Point> contour )
{
    double sum = 0;
    for( size_t i = 1; i < contour.size(); i++ )
    {
        // cout << i << ". dist " << distanceBtwPoints( contour[i], contour[i-1] ) << endl;
        sum += distanceBtwPoints( contour[i], contour[i-1] );
    }

    return sum ;
}

static Rect fitEllipse_check( vector<Point> contour )
{
    Mat pointsf;
    Mat(contour).convertTo(pointsf, CV_32F);
    RotatedRect box = fitEllipse(pointsf);
    //cout << box.size << "  " << box.size <<endl;
    return box.boundingRect();
    if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*2 )
        return Rect(-1,-1,0,0);
    else
        return box.boundingRect();
}

static Point moment_check( vector<Point> contour )
{
    /// Get the moment
    Moments mu;
    mu = moments( contour, false );

    ///  Get the mass centers:
    Point2f mc;
    mc = Point2f( static_cast<float>(mu.m10/mu.m00), static_cast<float>(mu.m01/mu.m00) );

    return Point(mc.x,mc.y);
}

static double avg_cosine( vector<Point> contour )
{
    double avg_cos = 0;
    if( contour.size() < 8 ) return avg_cos;
    Rect r0,r1;
    Rect2f rDiff;
    vector<Point> pts(3);

    r0 = fitEllipse_check( contour );
    r1 = boundingRect( contour );

    pts[0].x = r0.x + (r0.width / 2);
    pts[0].y = r0.y + (r0.height / 2);

    pts[1].x = r1.x + (r1.width / 2);
    pts[1].y = r1.y + (r1.height / 2);

    pts[2] = moment_check( contour );

    rDiff = boundingRect( pts );
    double diffRatio = ((rDiff.width / r1.width) + (rDiff.height / r1.height));
    //cout << "moments\n" << pts[1] << endl << pts[1] << endl << pts[2] << endl << diffRatio << endl;
    //    cout << "***********************   " << contour.size() << "  " << sum_distanceBtwPoints(contour) <<endl;
    if( diffRatio > 0.1) return avg_cos;

    for( size_t i = 2; i < contour.size(); i++ )
    {
        double cosine = fabs(angle(contour[i], contour[i-2], contour[i-1]));

        double dist0 = distanceBtwPoints( contour[i], contour[i-1] );
        double dist1 = distanceBtwPoints( contour[i-1], contour[i-2] );

        double diff = std::sqrt((dist0 - dist1) * (dist0 - dist1));


        if( diff < dist0 * 0.4)
            avg_cos += cosine;
        // cout << "avg_cos = " << avg_cos << "   " << i <<endl;

    }
    //  cout << "return avg_cos = " << avg_cos / (contour.size()-2) <<endl;
    return avg_cos / (contour.size()-2) ;
}
// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findObjects( Mat image, vector<Rect>& objects )
{
    objects.clear();
    Mat gray;
    cvtColor( image, gray, COLOR_BGR2GRAY );
    adaptiveThreshold(gray,gray,255,0,0,15,3);
    //dilate(gray, gray, Mat(), Point(-1,-1));
    erode( gray, gray, Mat(), Point(-1,-1));

    // find contours and store them all as a list
    vector<vector<Point> > contours;
    findContours(gray, contours, RETR_LIST, CHAIN_APPROX_NONE);

    vector<Point> approx;

    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, 6, true);
        double avg_cosine_ = avg_cosine( approx );

        if( (approx.size() > 7 && approx.size() < 20 && avg_cosine_ > 0.45 ))// ||
            //       (approx.size() > 16 && approx.size() < 20 && avg_cosine_ > 0.80) )
        {
            //   cout << "**   " << avg_cosine_ << "   " << approx.size() <<endl;

            Rect boundingRect_ = boundingRect(approx);
            objects.push_back( boundingRect_ );

           bool showDebug = true; // change it true to see detailed drawings
           if( showDebug )
            {
                rectangle( image, fitEllipse_check( approx ),Scalar( (rand()&255), (rand()&255), (rand()&255) ),1 );
                rectangle( image, boundingRect_,Scalar(0,0,255),1);
                polylines( image, approx,true,Scalar(0,127,255),2 );
                circle( image, moment_check( approx ), 4, Scalar(255,0,0), -1, 8, 0 );
            }

        }
    }
}


int main(int argc, char** argv)
{
    cout <<
         "Call:\n" << argv[0] << " [filename]\n" <<
         "Using OpenCV version " << CV_VERSION << "\n" << endl;

    namedWindow( wndname, 1 );
    vector<Rect> objects;
    Mat image = imread(argv[1], 1);
    if( !image.empty() )
    {
        findObjects( image, objects );
        for( size_t i = 0; i < objects.size(); i++ )
        {
            rectangle( image, objects[i], Scalar( (rand()&255), (rand()&255), (rand()&255) ),2  );
        }

        imshow("result", image );
        waitKey(0);
    }

    VideoCapture capture(0);

    int frameCounter = 0;
    int tick = 0;
    int fps;
    std::time_t timeBegin = std::time(0);

    for(;;)
    {
        capture >> image;

        imshow("Source", image );

        frameCounter++;

        std::time_t timeNow = std::time(0) - timeBegin;

        if (timeNow - tick >= 1)
        {
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }


        findObjects( image, objects );

        for( size_t i = 0; i < objects.size(); i++ )
        {
            rectangle( image, objects[i], Scalar( (rand()&255), (rand()&255), (rand()&255) ), 2  );
        }

        cv::putText(image, cv::format("Average FPS=%d", fps), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255));

        imshow("result", image );
        waitKey(30);
    }
    return 0;
}
