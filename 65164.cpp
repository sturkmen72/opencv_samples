// experimental code
// http://answers.opencv.org/question/65164

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(Point pt1, Point pt2, Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"65164.png";
    Mat img = imread(filename);
    if (img.empty())
        return -1;

    imshow("Source", img);
    moveWindow("Source",40,40);
    Mat src,gray;

    resize(img, src, Size(img.cols/1, img.rows/1)); // resize image to fit display

    cvtColor(src, gray, CV_BGR2GRAY); // Convert to grayscale

    // Use Canny instead of threshold to catch squares with gradient shading
    Mat bw;
    Canny(gray, bw, 0, 50, 5);

    // Find contours
    vector<vector<Point> > contours;
    findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    vector<Point> squares;
    vector<Point> approx;
    Mat dst = src.clone();

    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // Skip small or non-convex objects
        if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
            continue;

        if (approx.size() == 4 )
        {
            // Number of vertices of polygonal curve
            int vtc = approx.size();

            // Get the cosines of all corners
            vector<double> cos;
            for (int j = 2; j < vtc+1; j++)
                cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

            // Sort ascending the cosine values
            sort(cos.begin(), cos.end());

            // Get the lowest and the highest cosine
            double mincos = cos.front();
            double maxcos = cos.back();

            if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)

            {
                for(int j=0; j<4 ; j++ )
                    squares.push_back(approx[j]);

                drawContours( src, contours, i, Scalar(0,0,255), CV_FILLED, 8 );
            }
        }
    }

    Rect box = boundingRect(Mat(squares));
    rectangle(src,box,Scalar(0,255,0),1);

    imwrite("result-65164.jpg",src);
    imshow("result", src);
    moveWindow("result",img.cols/2,100);
    waitKey(0);
    return 0;
}
